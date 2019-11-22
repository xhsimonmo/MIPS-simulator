lui $3,0x2200
addi $2,$2,0xff11
sw $2,0($3) #$2 is ffffff11
lwl $2,2($3)#$2 is ff11ff11
srl $2,$2,24 #0x0000f1
jr $0

# $2 is 241 
