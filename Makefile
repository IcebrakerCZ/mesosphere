BINARY   = main
SOURCES  = $(wildcard *.cpp)
OBJECTS  = $(SOURCES:.cpp=.o)
DEPS     = $(SOURCES:.cpp=.dep)

CXXFLAGS = -std=c++0x -Wall -Werror -O2 -ggdb3
LDFLAGS  = -lboost_program_options -lboost_thread -lboost_date_time -lboost_system -lpthread


.PHONY: all
all: $(BINARY)


.PHONY: clean
clean:
	rm -f $(BINARY) $(OBJECTS) $(DEPS)


.PHONY: run-gdb run-valgrind
run-gdb run-valgrind: run

run-gdb     : RUN_COMMAND = gdb
run-valgrind: RUN_COMMAND = valgrind -v --leak-check=full --leak-resolution=high --num-callers=50


.PHONY: run
run:
	$(RUN_COMMAND) ./$(BINARY)


-include $(DEPS)


$(BINARY): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(LDFLAGS)


$(OBJECTS): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -M -o $*.dep $<
	$(CXX) $(CXXFLAGS) -c -o $@ $<
