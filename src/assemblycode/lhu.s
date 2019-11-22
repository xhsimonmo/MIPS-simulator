lui $3,0x2000
addiu $4,$0,0xfe00;
sw $4,0($3)
lhu $2,0($3)
jr $0

# result is 255
