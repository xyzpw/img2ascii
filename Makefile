GXX = g++
TARGET = img2ascii

SOURCES = src/*.cpp
HEADERS = include/*.hpp

CXXFLAGS = -std=c++20 -Iinclude -Wall
LDFLAGS = -lgif

$(TARGET): $(SOURCES) $(HEADERS)
	$(GXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)
