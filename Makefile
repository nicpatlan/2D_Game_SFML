CC=g++
CFLAGS=-c -Wall -std=c++17 -I/opt/homebrew/Cellar/sfml/2.6.1/include
LDFLAGS=-L/opt/homebrew/Cellar/sfml/2.6.1/lib
LIBFLAGS=-lsfml-graphics -lsfml-window -lsfml-system
SOURCES=sfml.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=main

all: $(SOURCES) $(EXECUTABLE)
        
$(EXECUTABLE): $(OBJECTS)
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBFLAGS)

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm *.o $(EXECUTABLE)
