#result is 10
lui $3, 0x2000
addiu $2, $0, 10
sb $2, 4($3)
jr $0