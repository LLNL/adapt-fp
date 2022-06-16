TARGETS=arclength arclength-float arclength-adapt arclength-mixed
CXXFLAGS+=-g -O3 -Wall
LDFLAGS=-lm
ADAPTFLAGS=--std=c++11 -I$(CODIPACK_HOME)/include -I../ -DCODI_ReversalZeroesAdjoints=0

all: $(TARGETS)

test: all
	-@echo "== Original (all double precision) =="
	-@./arclength
	-@echo ""
	-@echo "== Incorrect (all single precision) =="
	-@./arclength-float
	-@echo ""
	-@echo "== w/ ADAPT instrumentation =="
	-@./arclength-adapt
	-@echo ""
	-@echo "== Mixed precision version =="
	-@./arclength-mixed
	-@echo ""

diff:
	-@echo "== Diff between original and ADAPT version =="
	-@diff arclength.cpp arclength-adapt.cpp
	-@echo ""
	-@echo "== Diff between original and mixed version =="
	-@diff arclength.cpp arclength-mixed.cpp
	-@echo ""


arclength: arclength.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

arclength-float: arclength-float.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

arclength-adapt: arclength-adapt.cpp
	$(CXX) $(CXXFLAGS) $(ADAPTFLAGS) -o $@ $^ $(LDFLAGS)

arclength-mixed: arclength-mixed.cpp
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -rf $(TARGETS)

