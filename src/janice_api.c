#include "janice_api.h"
#include "../third_party/cJSON/cJSON.h"
#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// ----------------------------
// Helpers
// ----------------------------

// format number with commas (e.g. 1234567 -> "1,234,567")
static void format_with_commas(long long value, char *buf, size_t bufsize) {
	char tmp[64];
	snprintf(tmp, sizeof(tmp), "%lld", value);

	int len = strlen(tmp);
	int commas = (len - 1) / 3;
	int newlen = len + commas;

	if ((size_t)newlen >= bufsize) {
		buf[0] = '\0';
		return;
	}

	buf[newlen] = '\0';
	int i = len - 1, j = newlen - 1, count = 0;

	while (i >= 0) {
		if (count == 3) {
			buf[j--] = ',';
			count = 0;
		}
		buf[j--] = tmp[i--];
		count++;
	}
}

char *minerals_to_janice_body(const MineralDB *db) {
	// rough upper bound: each line ~64 chars
	size_t bufsize = db->count * 80 + 1;
	char *body = malloc(bufsize);
	if (!body)
		return NULL;
	body[0] = '\0';

	for (int i = 0; i < db->count; i++) {
		char numbuf[64];
		format_with_commas(db->minerals[i].quantity, numbuf,
				   sizeof(numbuf));
		snprintf(body + strlen(body), bufsize - strlen(body), "%s %s\n",
			 numbuf, db->minerals[i].name);
	}
	return body;
}

// ----------------------------
// libcurl integration
// ----------------------------

struct Memory {
	char *data;
	size_t size;
};

static size_t write_callback(void *contents, size_t size, size_t nmemb,
			     void *userp) {
	size_t realsize = size * nmemb;
	struct Memory *mem = (struct Memory *)userp;

	char *ptr = realloc(mem->data, mem->size + realsize + 1);
	if (ptr == NULL)
		return 0;

	mem->data = ptr;
	memcpy(&(mem->data[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->data[mem->size] = '\0';

	return realsize;
}

int janice_appraise(const MineralDB *db, JaniceResult *out) {
	CURL *curl;
	CURLcode res;
	int ret = -1;

	const char *janice_key = getenv("JANICE_API_KEY");

	if (!janice_key) {
		fprintf(stderr,
			"Env variable '%s' not set. Skipping appraisal.\n",
			"JANICE_API_KEY");

		return -2;
	}

	char *body = minerals_to_janice_body(db);
	if (!body)
		return -1;

	struct Memory chunk = {0};

	curl = curl_easy_init();
	if (!curl) {
		free(body);
		return -1;
	}

	curl_easy_setopt(curl, CURLOPT_URL,
			 "https://janice.e-351.com/api/rest/v2/appraisal");
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, body);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(body));
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: text/plain");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

	res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
			curl_easy_strerror(res));
	} else {
		// Parse JSON
		cJSON *root = cJSON_Parse(chunk.data);
		if (root) {
			cJSON *code = cJSON_GetObjectItem(root, "code");
			cJSON *prices =
			    cJSON_GetObjectItem(root, "effectivePrices");

			if (cJSON_IsString(code) && prices) {
				strncpy(out->code, code->valuestring,
					sizeof(out->code));
				out->code[sizeof(out->code) - 1] = '\0';

				out->totalBuyPrice =
				    cJSON_GetObjectItem(prices, "totalBuyPrice")
					->valuedouble;
				out->totalSplitPrice =
				    cJSON_GetObjectItem(prices,
							"totalSplitPrice")
					->valuedouble;
				out->totalSellPrice =
				    cJSON_GetObjectItem(prices,
							"totalSellPrice")
					->valuedouble;

				ret = 0;
			}
			cJSON_Delete(root);
		}
	}

	curl_slist_free_all(headers);
	curl_easy_cleanup(curl);
	free(body);
	free(chunk.data);

	return ret;
}
