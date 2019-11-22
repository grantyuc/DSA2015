target = 3_4_1.out 3_4_2.out
CFLAGS = -O2 -Wall
CPPFLAGS = -O2 -std=c++11 -Wall

all: $(target)

%: %.c
	gcc $(CFLAGS) $< -o $@

%: %.cpp
	g++ $(CPPFLAGS) $< -o $@

3_4_1.out: hw3_1.cpp hw3.h
	g++ $(CPPFLAGS) $< hw3.h -o 3_4_1.out

3_4_2.out: hw3_2.cpp hw3.h
	g++ $(CPPFLAGS) $< hw3.h -o 3_4_2.out

run1:
	./3_4_1.out

run2:
	./3_4_2.out

clean:
	rm -rf $(target)
