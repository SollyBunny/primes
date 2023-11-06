.PHONY: build_tester

build_tester:
	cc test.c -O4 -ldl -o test

build_test:
	cc -O4 -shared -o src/$$NAME.so src/$$NAME.c  -ldl