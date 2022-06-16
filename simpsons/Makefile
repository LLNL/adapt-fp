TARGETS=simpsons simpsons-float simpsons-adapt simpsons-mixed
CXXFLAGS+=-g -O3 -Wall
LDFLAGS=-lm
ADAPTFLAGS=--std=c++11 -I$(CODIPACK_HOME)/include -I../ -DCODI_ReversalZeroesAdjoints=0

all: $(TARGETS)

test: all
	-@echo "== Original (all double precision) =="
	-@./simpsons
	-@echo ""
	-@echo "== Incorrect (all single precision) =="
	-@./simpsons-float
	-@echo ""
	-@echo "== w/ ADAPT instrumentation =="
	-@./simpsons-adapt
	-@echo ""
	-@echo "== Mixed precision version =="
	-@./simpsons-mixed
	-@echo ""

diff:
	-@echo "== Diff between original and ADAPT version =="
	-@diff simpsons.cpp simpsons-adapt.cpp
	-@echo ""
	-@echo "== Diff between original and mixed version =="
	-@diff simpsons.cpp simpsons-mixed.cpp
	-@echo ""


simpsons: simpsons.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

simpsons-float: simpsons-float.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

simpsons-adapt: simpsons-adapt.cpp
	$(CXX) $(CXXFLAGS) $(ADAPTFLAGS) -o $@ $^ $(LDFLAGS)

simpsons-mixed: simpsons-mixed.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(TARGETS)

