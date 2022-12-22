.text
main:
    # read new line
    li      $v0, 5          # read int
    syscall
    move 	$a0, $v0		# a0 = v0, passes the command input into recurse

    addi    $sp, $sp, -4    # open frame (1 word)
    sw      $ra, 0($sp)     # save ra

    jal recurse             # performs recursive operations, takes in a0 and returns v0

    lw      $ra, 0($sp)     # restore ra
    addi	$sp, $sp, 4		# collapse frame

    move    $a0, $v0        # a0 = v0, puts output in recurse into a0
    li      $v0, 1          # print int
    syscall

    jr      $ra

recurse:

    # s0 will store the value of n for the current call

    addi    $sp, $sp, -12    # open frame (2 words)
    sw      $ra, 8($sp)     # save ra
    sw      $s0, 4($sp)     # save s0

    beqz    $a0, _basecase  # if a0 = 0 then jump to the base case

    addi    $a0, $a0, -1    # decrement n by 1 and pass to next recurse call
    move    $s0, $a0        # s0 = a0, saves n-1 for the current call in s0
    
    jal     recurse         # recurse again, inputs the decremented n and outputs in v0

    # f(N) = 3(n-1) + f(n-1) + 1
    # 3*s0 + v0 + 1
    mul     $s0, $s0, 3     # s0 *= 3
    addi    $s0, $s0, 1     # s0 += 1
    add     $v0, $v0, $s0   # v0 += s0

    lw      $s0, 4($sp)     # restores s0
    lw      $ra, 8($sp)     # restore ra
    addi    $sp, $sp, 12    # collapse frame

    jr      $ra

_basecase:

    lw      $s0, 4($sp)     # restores s0
    lw      $ra, 8($sp)     # restore ra
    addi    $sp, $sp, 12    # collapse frame

    # set v0 = 2
    li      $v0, 2

    jr      $ra

.data
# print a new line
str_newline: .asciiz "\n"