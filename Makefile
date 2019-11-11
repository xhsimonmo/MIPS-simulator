simulator: simulator.o main.o
	g++ simulator.o  main.o  -o mips_simulator

simulator.o: simulator.cpp simulator.hpp
	g++ -c simulator.cpp

main.o: main.cpp
	g++ -c main.cpp

run:
	./mips_simulator
#./prog.exe if Windows/Cygwin

clean:
	rm *.o prog
# rm *.o prog.exe if Windows/Cygwin

#structure
#target: [dependencies list ...]
#	commands #starts with a tabulation
