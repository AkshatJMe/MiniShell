# Makefile for MiniShell

CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -g -O2
LDFLAGS = -lreadline

TARGET = minishell
SOURCE = minishell.cpp

# Default target
all: $(TARGET)

# Build the executable
$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE) $(LDFLAGS)
	@echo "🎉 MiniShell compiled successfully!"
	@echo "🚀 Run with: ./$(TARGET)"

# Clean build artifacts
clean:
	rm -f $(TARGET) .minishell_history
	@echo "🧹 Cleaned build artifacts"

# Install readline library (Ubuntu/Debian)
install-deps:
	sudo apt-get update
	sudo apt-get install libreadline-dev
	@echo "📦 Dependencies installed for Ubuntu/Debian"

# Install readline library (macOS)
install-deps-mac:
	brew install readline
	@echo "📦 Dependencies installed for macOS"

# Run the shell
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean install-deps install-deps-mac run