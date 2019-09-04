# HPCCG example

HPCCG is a mini-application from the Mantevo benchmark suite (https://github.com/Mantevo/HPCCG).
It is a simple conjugate benchmark code.

The example is provided as sub-directories with Makefile for automation.

## Compiling

Once inside the sub-directory, run `make` to build the example. You must have 
CoDiPack accessible in `CODIPACK_HOME`.

## Testing

Run `make; make test` inside 1) HPCCG/ to run the original all-double-precision
version showing the correct output, 2) HPCCG-adapt/ to run the ADAPT version
where the output shows the recommendation to convert the original variables, 3)
HPCCG-adapt-iteration/ to run the ADAPT version which does the analysis at the
iteration level and generates an .pdf output file with the results of the
analysis, and 4) the final mixed precision version built based on ADAPT's
recommendation.

