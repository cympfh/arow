test: test1 test2 test3

test1: arow
	./arow -r 0.1     < sample/linear > sample/linear.model
	./arow -f sample/linear.model --test < sample/linear > sample/linear.out
	./arow -D -r 0.1     < sample/linear > sample/linear.diag.model
	./arow -f sample/linear.diag.model --test < sample/linear > sample/linear.diag.out

test2: arow
	./arow -r 0.01 < sample/noise  > sample/noise.model
	./arow -f sample/noise.model --test < sample/noise > sample/noise.out
	./arow -D -r 0.01 < sample/noise  > sample/noise.diag.model
	./arow -f sample/noise.diag.model --test < sample/noise > sample/noise.diag.out

test3: arow
	./arow -r 0.1 < sample/sparse > sample/sparse.model
	./arow -f sample/sparse.model --test < sample/sparse > sample/sparse.out

arow: main.cc parse.cc arow.hh arow.cc
	g++ -std=c++11 main.cc -o $@

clean:
	rm ./arow
