target = finalbank
CFLAGS = -O2 -Wall
CPPFLAGS = -O0 -std=c++11 -Wall -g
CPPFLAGS = -O2 -std=c++11 -Wall

all: $(target)

%: %.c
	gcc $(CFLAGS) $< -o $@

%: %.cpp
	g++ $(CPPFLAGS) $< -o $@

finalbank: finalbank.cpp User.h User.cpp md5.h md5.cpp findID.h
	g++ $(CPPFLAGS) $< User.cpp md5.cpp findID.h -o final_project

run:
	./finalbank

clean:
	rm -rf $(target)

cache:
	git config --global credential.helper cache

