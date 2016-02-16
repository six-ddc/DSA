all:
	gcc -g -o bin_test test.c 
run: all
	./bin_test
clean:
	# rm -rf bin_c_test bin_c_test.dSYM > /dev/null 2>&1
	git clean -fXd
