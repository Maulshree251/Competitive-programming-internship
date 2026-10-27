CXX = g++
CXXFLAGS = -std=c++14 -O2 -Wall -Wextra -Iinclude

SRCDIR = src
OBJDIR = obj
BIN = rat_in_maze.exe

SRCS = $(SRCDIR)/Maze.cpp $(SRCDIR)/MazeSolver.cpp $(SRCDIR)/main.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(BIN)

$(BIN): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(SRCDIR)/*.o $(BIN)

.PHONY: all clean
