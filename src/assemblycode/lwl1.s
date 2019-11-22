lui $3,0x2000
addi $2,$2,0xff11
sw $2,0($3) #$2 is ffffff11
lwl $2,2($3)
jr $0

# $2 is 17 
