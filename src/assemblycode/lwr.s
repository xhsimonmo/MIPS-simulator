lui $3,0x2000
addi $4,$4,0xff11
sw $4,0($3) #$2 is ffffff11
lwr $2,2($3)#$2 is 0000ff11
jr $0

# $2 is 17
