# Autor: Kamil Zieliński
# Makefile dla clienta

CC=gcc
CXXFLAGS=-c
LDFLAGS=
CSOURCES=client.cpp connectionn.cpp 
SOURCES=$(CSOURCES) 
OBJECTS=$(CSOURCES:.c=.o)
EXECUTABLE=client

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(LDFLAGS) $(OBJECTS) -o $@ $(LIBS)

.cpp.o:
	$(CXX) $(CXXFLAGS) $< -o $@

clean:
	rm *.o

