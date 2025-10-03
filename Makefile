CC       = clang
CFLAGS   = -Wall -Wextra -g
INCLUDES = -Isrc -Ithird_party/cJSON
LDLIBS   = -lm -lcurl

# Sources
SRC     = src/yield_table.c \
	  src/calculator.c \
	  src/janice_api.c \
	  src/parser.c \
	  src/main.c \
	  third_party/cJSON/cJSON.c

TEST_SRC = tests/test_calculator.c \
	   tests/test_janice_api.c \
	   tests/test_yield_table.c \
	   tests/test_parser.c \
	   tests/test_runner.c \
	   src/calculator.c \
	   src/janice_api.c \
	   src/parser.c \
	   src/yield_table.c

CHECK_CFLAGS := $(shell pkg-config --cflags check)
CHECK_LIBS   := $(shell pkg-config --libs check)

# Executables
TARGET  = metenox
TESTBIN = metenox_tests

# Default target
all: $(TARGET)

# Build main program
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(INCLUDES) -o $@ $^ $(LDLIBS)

# Build tests with Check
$(TESTBIN): $(TEST_SRC) third_party/cJSON/cJSON.c
	$(CC) $(CFLAGS) $(INCLUDES) $(CHECK_CFLAGS) -o $@ $^ $(CHECK_LIBS) -lpthread -lm -lcurl

# Run unit tests
check: $(TESTBIN)
	./$(TESTBIN)

# Clean build artifacts
clean:
	rm -f $(TARGET) $(TESTBIN)

