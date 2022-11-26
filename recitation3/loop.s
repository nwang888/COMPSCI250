.text

main:
      li $t0, 0
      li $t1, 11

      start_loop:
            li $v0, 1     # tells syscall that it wants to print int https://courses.missouristate.edu/kenvollmar/mars/help/syscallhelp.html
            move $a0, $t0 # move integer into argument
            syscall       # print

            li $v0, 4     # tells syscall to print a string ("\n")
            la $a0, nln   # load address of newline into argument
            syscall       # print

            addi $t0, $t0, 1

            blt $t0, $t1, start_loop

       jr $ra

.data
nln: .asciiz "\n"

name_buffer: .space 64

# this is a comment
