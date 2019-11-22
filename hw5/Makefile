target = system
CFLAGS = -O2 -Wall
CPPFLAGS = -O0 -std=c++11 -Wall -g
CPPFLAGS = -O2 -std=c++11 -Wall

all: $(target)

%: %.c
	gcc $(CFLAGS) $< -o $@

%: %.cpp
	g++ $(CPPFLAGS) $< -o $@

system: system.cpp binomial_heap.h
	g++ $(CPPFLAGS) $< binomial_heap.h -o $@

run:
	./system

clean:
	rm -rf $(target)

cache:
	git config --global credential.helper cache

