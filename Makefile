SHELL := /bin/bash

# For simulator
CC = g++
CPPFLAGS = -W -Wall

# For MIPS binaries. Turn on all warnings, enable all optimisations and link everything statically
MIPS_CC = mips-linux-gnu-gcc
MIPS_OBJCOPY = mips-linux-gnu-objcopy
MIPS_OBJDUMP = mips-linux-gnu-objdump
MIPS_CPPFLAGS = -W -Wall -O3 -fno-builtin -march=mips1 -mfp32
MIPS_LDFLAGS = -nostdlib -Wl,-melf32btsmip -march=mips1 -nostartfiles -mno-check-zero-division -Wl,--gpsize=0 -static -Wl,-Bstatic -Wl,--build-id=none

# Compile C file (.c) into MIPS object file (.o)
%.mips.o: %.c
	$(MIPS_CC) $(MIPS_CPPFLAGS) -c $< -o $@

# Assemble MIPS assembly file (.s) into MIPS object file (.o)
%.mips.o: %.s
	$(MIPS_CC) $(MIPS_CPPFLAGS) -c $< -o $@

# Link MIPS object file (.o), producing .elf, using memory locations specified in spec
%.mips.elf: %.mips.o
	$(MIPS_CC) $(MIPS_CPPFLAGS) $(MIPS_LDFLAGS) -T linker.ld $< -o $@

# Extract binary instructions only from linked object file (.elf)
%.mips.bin: %.mips.elf
	$(MIPS_OBJCOPY) -O binary --only-section=.text $< $@

# Disassemble linked object file (.elf), pulling out instructions as MIPS assembly file (.s)
%.mips.s : %.mips.elf
	$(MIPS_OBJDUMP) -j .text -D $< > $@



#////////////////////////////////////////////////////////////////////////////////////////////////////
# assembly test file are in folder "assemblycode"; we use wildcard function to load them one by one
# @basename takes away the extension

assem_address = ./src/assemblycode
bin_address = ./src/genbinary
disassem_address = ./src/disassembly
buffer_address = ./src/buffer

testnames = $(basename $(wildcard $(assem_address)/*.s)) # Extract every name of assembly test code for later use#

# .NOTPARALLEL: $(testnames)
emm:
	echo $(testnames)


.PHONY testbench: $(testnames) #

testbench:
# $(testnames):
# 	$(MIPS_CC) $(MIPS_CPPFLAGS) -c $@.s -o $@.mips.o #assembly file to object infile
#
# 	$(MIPS_CC) $(MIPS_CPPFLAGS) $(MIPS_LDFLAGS) -T linker.ld $@.mips.o -o $@.mips.elf # to elf
# 	$(MIPS_OBJCOPY) -O binary --only-section=.text $@.mips.elf $(bin_address)/$(notdir $@).mips.bin
$(testnames):
	@$(MIPS_CC) $(MIPS_CPPFLAGS) -c $@.s -o $(buffer_address)/$(notdir $@).mips.o #assembly file to object infile
	@$(MIPS_CC) $(MIPS_CPPFLAGS) $(MIPS_LDFLAGS) -T linker.ld $(buffer_address)/$(notdir $@).mips.o -o $(buffer_address)/$(notdir $@).mips.elf # to elf
	@$(MIPS_OBJCOPY) -O binary --only-section=.text $(buffer_address)/$(notdir $@).mips.elf $(bin_address)/$(notdir $@).mips.bin
	# $(MIPS_OBJDUMP) -j .text -D $@.mips.elf > $(disassem_address)/$(notdir $@).mips.disassembly.s



# for simulator building


simulator: simulator.o main.o
	mkdir -p bin
	g++  simulator.o  main.o  -o bin/mips_simulator

simulator.o: simulator.cpp simulator.hpp
	g++ -c simulator.cpp

main.o: main.cpp
	g++ -c main.cpp

run:
	./bin/mips_simulator ADDU.bin
#./prog.exe if Windows/Cygwin

clean:
	rm *.o bin/mips_simulator
# rm *.o prog.exe if Windows/Cygwin

#structure
#target: [dependencies list ...]
#	commands #starts with a tabulation
