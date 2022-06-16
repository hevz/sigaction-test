check : test
	./test read && echo PASS
	./test sigreturn && echo PASS
	./test sigsuspend && echo PASS

test : test.c check.S
	gcc -O3 -o $@ $^ -pthread

clean :
	rm -f test
