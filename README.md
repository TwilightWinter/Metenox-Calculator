# Metenox Calculator

This is a Metenox Moon drill value calculator which runs on the command line.

## Platforms

### Mac/Linux

The tool is confirmed to work on MacOS, and should work on Linux.

### Windows

There is no reason that the tool should not work on Windows, however:

- It depends on libcurl, which can be a pain to build on Windows
- I do not have a Windows environment on which to build the tool

If someone provides a pull request to build on Windows, I will be happy to accept it.

I think that this will be trivial for anyone who regularly works with C on Windows.

## Building

You should be able to just head to the CLI and run:

```
make
```

This will compile the tool.

## Running

### On CLI

Running the tool is as simple as:

```
./metenox moons.txt
```

This will depend on a valid moons.txt in the project directory.

### Running Appraisals

You can run an appraisal for each moon by setting a Janice key, which can be obtained [on Discord](https://janice.e-351.com/about):

```
export JANICE_API_KEY=xxxxxxxxxxx
./metenox moons.txt
```

If this is not set, the script will still run but you will have to manually run appraisals.

### moons.txt format

The input format is exactly what comes from the probing tool:

```
ABC-12 I - Moon 1
    Bitumens      0.196226656437    45492    30004050    40256430    40256439
    Vanadinite    0.432646811008    45500    30004050    40256430    40256439
    Zeolites      0.371126532555    45490    30004050    40256430    40256439

ABC-12 VII - Moon 2
    Sperrylite    0.288101166487    45499    30004053    40256714    40256724
    Sylvite       0.280994147062    45491    30004053    40256714    40256724
    Titanite      0.230904683471    45496    30004053    40256714    40256724
```

## Credit

This tool was developed by [Twilight Winter](https://zkillboard.com/character/96628235/).

If you find it useful, feel free to send an ISK donation.

Also, OOTE is recruiting!
