
src/assemblycode/SW-putc.mips.elf:     file format elf32-tradbigmips


Disassembly of section .text:

10000000 <.text>:
10000000:	3c023000 	lui	v0,0x3000
10000004:	24030058 	li	v1,88
10000008:	03e00008 	jr	ra
1000000c:	ac430004 	sw	v1,4(v0)
10000010:	00000000 	nop
10000014:	3c023000 	lui	v0,0x3000
10000018:	03e00008 	jr	ra
1000001c:	ac440004 	sw	a0,4(v0)
	...
