TARGETS=sum2pi_x sum2pi_x-float sum2pi_x-adapt sum2pi_x-mixed
CXXFLAGS+=-g -O3 -Wall
LDFLAGS=-lm
ADAPTFLAGS=--std=c++11 -I$(CODIPACK_HOME)/include -I../ -DCODI_ReversalZeroesAdjoints=0

all: $(TARGETS)

test: all
	-@echo "== Original (all double precision) =="
	-@./sum2pi_x
	-@echo ""
	-@echo "== Incorrect (all single precision) =="
	-@./sum2pi_x-float
	-@echo ""
	-@echo "== w/ ADAPT instrumentation =="
	-@./sum2pi_x-adapt
	-@echo ""
	-@echo "== Mixed precision version =="
	-@./sum2pi_x-mixed
	-@echo ""

diff:
	-@echo "== Diff between original and ADAPT version =="
	-@diff sum2pi_x.cpp sum2pi_x-adapt.cpp
	-@echo ""
	-@echo "== Diff between original and mixed version =="
	-@diff sum2pi_x.cpp sum2pi_x-mixed.cpp
	-@echo ""


sum2pi_x: sum2pi_x.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

sum2pi_x-float: sum2pi_x-float.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

sum2pi_x-adapt: sum2pi_x-adapt.cpp
	$(CXX) $(CXXFLAGS) $(ADAPTFLAGS) -o $@ $^ $(LDFLAGS)

sum2pi_x-mixed: sum2pi_x-mixed.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(TARGETS)

