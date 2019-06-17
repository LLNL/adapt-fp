TARGETS=sanity sanity-float sanity-adapt sanity-mixed
CXXFLAGS+=-g -O0 -Wall -Wno-unknown-pragmas
ADAPTFLAGS=--std=c++11 -I$(CODIPACK_HOME)/include -I../

all: $(TARGETS)

test: all
	-@echo "== Original (all double precision) =="
	-@./sanity
	-@echo ""
	-@echo "== Incorrect (all single precision) =="
	-@./sanity-float
	-@echo ""
	-@echo "== w/ ADAPT instrumentation =="
	-@./sanity-adapt
	-@echo ""
	-@echo "== Mixed precision version =="
	-@./sanity-mixed
	-@echo ""

diff:
	-@echo "== Diff between original and ADAPT version =="
	-@diff sanity.cpp sanity-adapt.cpp
	-@echo ""
	-@echo "== Diff between original and mixed version =="
	-@diff sanity.cpp sanity-mixed.cpp
	-@echo ""

sanity: sanity.cpp
	$(CXX) -o $@ $^

sanity-float: sanity-float.cpp
	$(CXX) -o $@ $^

sanity-adapt: sanity-adapt.cpp
	$(CXX) $(ADAPTFLAGS) -o $@ $^

sanity-mixed: sanity-mixed.cpp
	$(CXX) -o $@ $^

clean:
	rm -rf $(TARGETS)

