addiu $10,$0,0x2000
sll $10,$10,16 #$10 = 0x20000000
addiu $3,$0,100
sb $3,0($10)
lb $2,0($10)
jr $0

# $2 is 0