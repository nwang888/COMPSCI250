.text

main:
        li $v0, 5 #read integer from command line
        syscall

        move $t0, $v0 # move input to $t0, this is the command line input
        addi $t0, $t0, 1 # increment $t0 so that we can print with index = 1 instead of 0

        li $t1, 1 #instantiate t1 as a counter for the loop
        li $t3, 7 #instatiate t3 as what the loop spits out

_loop:
        slt $t2, $t1, $t0  # loop condition if $t1 < $t0 = 5 then t2 = 1, ow t2 = 0
        beqz $t2, _endloop  # if $t2 == 0 then exit loop

        li $v0, 1 # print number
        move $a0, $t3
        syscall

        li $v0, 4  # print string: newline
        la $a0, str_newline
        syscall

        addi $t1, $t1, 1 #increment t1
        addi $t3, $t3, 7 #increment t3
        
        j _loop # branch unconditionally

_endloop:
        li $v0, 0
        jr $ra

.data
# print a new line
str_newline: .asciiz "\n"