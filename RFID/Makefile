# Makefile for RFID Reader Project

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall

# Target names
TARGET = rfid_reader

# Source files
SRCS = main.cpp rfid_utils.cpp

# Libraries to link
LIBS = -lnfc

# Directories
VENV_DIR = ../Spotify/.venv

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(LIBS)

run: all
	bash -c "source $(VENV_DIR)/bin/activate && ./$(TARGET) 2> >(grep -v 'Unable to wait for SPI data' >&2)"
clean:
	rm -f $(TARGET)

