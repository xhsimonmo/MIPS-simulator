simulator: simulator.o main.o
        mkdir -p bin
	g++ simulator.o  main.o  -o bin/mips_simulator

simulator.o: simulator.cpp simulator.hpp
	g++ -c simulator.cpp

main.o: main.cpp
	g++ -c main.cpp

run:
	./bin/mips_simulator test.bin
#./prog.exe if Windows/Cygwin

clean:
	rm *.o bin/mips_simulator
# rm *.o prog.exe if Windows/Cygwin

#structure
#target: [dependencies list ...]
#	commands #starts with a tabulation
