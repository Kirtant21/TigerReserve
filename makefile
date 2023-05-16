CXX = g++
CXXFLAGS = -Wall

mytest.o: streak.o mytest.cpp
	$(CXX) $(CXXFLAGS) streak.o mytest.cpp -o mytest
streak.o: streak.cpp streak.h
	$(CXX) $(CXXFLAGS) -c streak.cpp

run:
	./mytest
val:
	valgrind ./mytest
fullCheck:
	valgrind --leak-check=full ./mytest
debug:
	gdb ./mytest
clean:
	rm *.o*
	rm *~
	rm *#
s:
	emacs streak.cpp
sh:
	emacs streak.h
t:
	emacs mytest.cpp
mk:
	emacs makefile
submit:
	cp streak.h streak.cpp mytest.cpp ~/cs341proj/proj2/
chkSub:
	ls ~/cs341proj/proj2/
