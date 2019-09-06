all:
	make -C HPCCG
	make -C HPCCG-adapt
	make -C HPCCG-adapt-iteration
	make -C HPCCG-mixed

test:
	-@echo ""
	-@echo "== Original (all double precision) =="
	-@echo ""
	make -C HPCCG test
	-@echo ""
	-@echo ""
	-@echo "== w/ ADAPT normal instrumentation =="
	-@echo ""
	make -C HPCCG-adapt test
	-@echo ""
	-@echo ""
	-@echo "== w/ ADAPT iteration-based instrumentation =="
	-@echo ""
	make -C HPCCG-adapt-iteration test
	-@echo ""
	-@echo ""
	-@echo "== Mixed precision version =="
	-@echo ""
	make -C HPCCG-mixed test

clean:
	make -C HPCCG clean
	make -C HPCCG-adapt clean
	make -C HPCCG-adapt-iteration clean
	make -C HPCCG-mixed clean
