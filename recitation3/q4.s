.text

main:
        li $t0, 1
_loop:
        slti $t2, $t0, 11   # if $t1 < 11 then t2 = 1
        beqz $t2, _endloop  # $t2 == 0 ; else 

        mul $t1, $t0, $t0   # compute $t0*$t0

        li $v0, 1 #print number
        move $a0, $t0
        syscall

        li $v0, 4 #print space
        la $a0, str_space
        syscall

        li $v0, 1 #print square
        move $a0, $t1
        syscall

        li $v0, 4  # print newline
        la $a0, str_newline
        syscall

        addi $t0, $t0, 1 #increment t0
        j _loop #branch unconditionally

_endloop:
        li $v0, 0
        jr $ra

.data
# print a space
str_space: .asciiz " "
str_newline: .asciiz "\n"
