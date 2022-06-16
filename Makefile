check : test
	./test && echo PASS

test : test.c check.S
	gcc -O3 -o $@ $^

clean :
	rm -f test
