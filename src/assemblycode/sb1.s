#result is -11(245)
lui $3, 0x1000 
addiu $2, $0, 16
sb $2, 5($3)
jr $0