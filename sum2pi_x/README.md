# Sum2pi_x example

This is an example from [CRAFT](https://github.com/crafthpc/craft/tree/master/demo/sum2pi_x).
The program
calculates PI\*X, where X is hard-coded for simplicity. The program uses an
unnecessarily computation-heavy method of calculating PI\*X for demonstration
purposes. There is an inner loop that sums to PI, and then an outer loop adds
that to itself X times. The sensitivity of the test at the end is adjustable
using the #define statements at the top. With the default value, double
precision produces a passing final value, while single precision does not.

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

