#
# This makefile builds and runs the provided demos. ADAPT itself does not
# compile separately; see README.md for more information about how to use it.
#

all:
	make -C sanity
	make -C sum2pi_x
	make -C arclength

test:
	make -C sanity test
	make -C sum2pi_x test
	make -C arclength test

clean:
	make -C sanity clean
	make -C sum2pi_x clean
	make -C arclength clean
