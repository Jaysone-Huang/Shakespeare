CXX = gcc
CXXFLags = -Wall

OBJECTS = shakespeare.o

shakespeare: $(OBJECTS)
	$(CXX) -g $(CXXFLAGS) -o shakespeare shakespeare.o

shakespeare.o: shakespeare.h shakespeare.c
	$(CXX) -g $(CXXFLAGS) -c shakespeare.c


clean:
	rm -f *.o
	rm shakespeare
