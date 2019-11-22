#result is (-11)245
lui $3, 0x2000
addi $2, $0, 5
sh  $2, 1($3)
addi $2, $0, 5
jr  $0