CPP_FLAGS = -Wall -std=c++11
DEBUG_FLAGS += -g -O0
PROG = graphviz_utility

INC = graphviz_utility.h
SRC = example.cpp

.PHONY: all
all: $(PROG)

.PHONY: debug
debug:
	g++-4.9 $(CPP_FLAGS) $(DEBUG_FLAGS) $(SRC) -o $(PROG)

.PHONY: clean
clean:
	rm $(PROG)

$(PROG): $(SRC) $(INC)
	g++-4.9 -O2 $(CPP_FLAGS) $(SRC) -o $(PROG)
