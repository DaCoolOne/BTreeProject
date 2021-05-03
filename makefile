

default: main

test:
	g++ -g unit_test_main.cpp -o testsuite
	./testsuite

.PHONY: clean, tests

# My two braincells sometimes add an s
tests: test

main:
	g++ driver.cpp
	./a.out
	python3 parse_display_results.py output.txt

clean:
	rm *.o
	rm testsuite
