CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -Iinclude

SRCDIR = src
INCDIR = include
BINDIR = bin

TARGET = $(BINDIR)/map_navigator
SRCS = $(SRCDIR)/main.cpp $(SRCDIR)/Graph.cpp $(SRCDIR)/NavigationSystem.cpp
OBJS = $(SRCS:$(SRCDIR)/%.cpp=$(BINDIR)/%.o)

.PHONY: all clean run

all: $(TARGET)

$(TARGET): $(OBJS) | $(BINDIR)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo "Build complete: $(TARGET)"

$(BINDIR)/%.o: $(SRCDIR)/%.cpp | $(BINDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

run: all
	./$(TARGET)

clean:
	rm -rf $(BINDIR)
