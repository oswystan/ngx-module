CPPFLAGS := -Wall -Werror
a.out: reg.cc
	g++ $(CPPFLAGS) -std=c++11 $^ -o $@
	./$@

clean:
	rm -f a.out
