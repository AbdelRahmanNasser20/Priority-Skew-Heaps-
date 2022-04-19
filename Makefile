XX = g++
CXXFLAGS = -Wall

proj3: squeue.o squeue.cpp
		$(CXX) $(CXXFLAGS) squeue.o mytest.cpp -o proj3

squeue.o: squeue.h squeue.cpp
		$(CXX) $(CXXFLAGS) -c squeue.cpp
run:
		proj3

val:
		valgrind proj3

clean:
		rm *~
		rm *.o
