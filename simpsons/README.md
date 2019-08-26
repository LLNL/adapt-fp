# Arclength example

This is an example from [Precimonious](https://github.com/corvette-berkeley/precimonious).
The program runs a discrete fourier transform. Several variables can be stored
in single precision.

The example is provided as four source files and a Makefile for automation.

## Compiling

Run `make` to build all forms of the example. You must have CoDiPack accessible
in `CODIPACK_HOME`.

## Testing

Run `make test` to run 1) the original all-double-precision version showing the
correct output, 2) an all-single-precision version to show that the result is
not the same, 3) the ADAPT output showing the recommendation to convert one of
the original variables, and 4) the final mixed precision version built based on
ADAPT's recommendation.

## Source diff

Run `make diff` to see the edits required to run ADAPT as well as the difference
between the original and the mixed-precision version.

