.text
main:
    addi $sp, $sp, -4
    sw $ra, 0($sp)

foo:
    addi    $sp, $sp, -8        # stack for t2 and ra
    add     $t2, $a0, $a1       # t2 = a0 + a1

    slti    $t4, 10, $t2        # if 10 < t2 (if a0 + a1 > 10) t4 == 1
    beqz    $t4, _else          # if t4 == 0 jump to else (recurse)

_basecase:                      # if BASE CASE
    move    $v0, $t2            # set return to t2
    addiu   $sp, $sp, 8         # moves stack up
    jr      $ra                 # return (ra has not changed)

_else:                          # RECURSE
    sw      $ra, 0($sp)         # ra saved on stack 
    sw      $t2, 4($sp)         # t2 saved on stack
    addi    $a0, $a0, 1         # increment a0 for next recursive call
    addi    $a1, $a1, 1         # increment a1 for next recursive call

    jal     foo                 # recursive call for foo

    lw      $t2, 4($sp)         # loads t2 back from stack
    lw      $ra, 0($sp)         # loads ra back from stack
    add     $v0, $v0, $t2

    addi    $sp, $sp, 8         # reset stack
    jr      $ra                 # returns
