simulator: simulator.o main_test.o
	g++ simulator.o  main_test.o  -o prog

simulator.o: simulator.cpp simulator.hpp
	g++ -c simulator.cpp


main.o: main_test.cpp
	g++ -c main_test.cpp

run:
	./prog test.bin
#./prog.exe if Windows/Cygwin

clean:
	rm *.o prog
# rm *.o prog.exe if Windows/Cygwin

#structure
#target: [dependencies list ...]
#	commands #starts with a tabulation
