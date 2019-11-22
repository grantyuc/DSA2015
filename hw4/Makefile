target = tree forest
CFLAGS = -O2 -Wall
CPPFLAGS = -O0 -std=c++11 -Wall -g
CPPFLAGS = -O2 -std=c++11 -Wall

FNAME = a1a.train
EPSILON = 0.1
N = 50

ta = Makefile.ta

all: $(target)

%: %.c
	gcc $(CFLAGS) $< -o $@

%: %.cpp
	g++ $(CPPFLAGS) $< -o $@

dtree: dtree.cpp DecisionTree.cpp DecisionTree.h
	g++ $(CPPFLAGS) $< DecisionTree.cpp -o dtree

tree: tree.cpp DecisionTree.cpp DecisionTree.h
	g++ $(CPPFLAGS) $< DecisionTree.cpp -o tree

forest: forest.cpp DecisionTree.cpp DecisionTree.h
	g++ $(CPPFLAGS) $< DecisionTree.cpp -o forest

run: run1 run2

run1:
	./tree $(FNAME) $(EPSILON) > tree_pred_func.cpp

run2:
	./forest $(FNAME) $(N) > forest_pred_func.cpp

clean:
	rm -rf $(target)

ta: $(ta)
	make -f $(ta) .PHONY

cache:
	git config --global credential.helper cache

