test: arow
	./arow -v -r 0.1 <sample/linear 2>log/linear.log >sample/linear.model
	./arow -v -r 0.01 <sample/noise 2>log/noise.log >sample/noise.model
	./arow -v -D -r 0.1 <sample/linear 2>log/linear.diag.log >sample/linear.diag.model
	./arow -v -D -r 0.01 <sample/noise 2>log/noise.diag.log >sample/noise.diag.model
	./arow -v -f sample/linear.model < sample/noise 2>log/linear2noise.log

arow: main.cc parse.cc arow.hh arow.cc
	g++ -std=c++11 main.cc -o $@

clean:
	rm ./arow
