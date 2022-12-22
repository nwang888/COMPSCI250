.text
.align 2

main:

# t9: head of the linkedlist
li      $t9, 0      # head = NULL

_read_input:
# function that reads lines from console and outputs the start address in heap that contains:
# the name of the pizza (char[]) and the ppd (float)

# REMEMBER: t4 = Start of this pizza in heap

    # allocate heap memory for this pizza
    li      $v0, 9          # allocate heap memory - string, float, pointer
    li      $a0, 72         # bytes = 64 + 4 + 4 = 72
    syscall

    # store start of heap
    move    $t4, $v0        # t4 = start of heap allocated

_read_string:
    # read name (string)

    # print prompt for pizza name
    la      $a0, prompt_name # load the prompt for the pizza name into a0
    li      $v0, 4
    syscall
    
    li      $v0, 8          # read string
    la      $a0, buffer     # read will be stored in a0 with size buffer
    li      $a1, 64         # allot the byte space for string
    syscall

    la      $t1, buffer     # t1 = address of buffer

    move    $t7, $t4        # copy t4 to t7 so we can use t7 to increment

    la      $t8, str_newline    # copy \n to t8
    lb      $t2, 0($t8)

    _save_string:
        # save the string in heap
        lb      $t0, 0($t1)       # Read one byte from the buffer into t0
        beq     $t2, $t0, _clear_newline # 
        sb      $t0, 0($t7)       # Store it at the location in memory
        addi    $t1, $t1, 1       # increment the source address to the next char
        addi    $t7, $t7, 1       # increment the destination address to the next char
        j       _save_string      # jump back to start of loop
    
    _clear_newline:
        # lb      $0, 0($t0)
        
    move    $t7, $t4         # copy t4 to t7 so we can use t7 to increment
    la      $t2, strDONE    # put "DONE" in t2

    _check_DONE:
        # check if we hit EOF - when it reads "DONE"
        # a0 still contains the source address of the string
        # t5 and t6 will be used to hold the current character for DONE and the pizza name

        lb      $t5, 0($t2)      # Read one byte from DONE into t5
        lb      $t6, 0($t7)      # Read one byte from str into s6
        addi    $t7, $t7, 1      # increment pizzaname to the next char
        addi    $t2, $t2, 1      # increment t2 (DONE) to the next char

        bne     $t5, $t6, _read_floats      # if the current bytes are not equal, this is a pizza name so jump to read the inputted flaots
        
        # only characters that are equal will make this section
        beqz     $t5, _exit             # if chars match up to be DONE and hits \n then exit loop
        # only chars that are equal but not the null terminator will go here
        j       _check_DONE      # go to next character in the string

_read_floats:

    # print prompt for diameter
    la      $a0, prompt_diameter # load the prompt for the diameter of the pizza into a0
    li      $v0, 4
    syscall
    
    # read diameter (float)
    li      $v0, 6          # read float
    syscall                 # outputs into f0
    mov.s    $f4, $f0        # save in f4

    # print prompt for cost
    la      $a0, prompt_cost     # load the prompt for the cost of the pizza into a0
    li      $v0, 4
    syscall

    # read cost (float)
    li      $v0, 6          # read float
    syscall                 # outputs into f0
    mov.s    $f5, $f0        # save in f5

    # edge case: if cost == 0, ppd = 0
    li.s    $f10, 0.0       # f10 = 0.0
    c.eq.s  $f5, $f10       # float flag = 1 if cost == 0
    bc1t    _edgecase       # jump to set ppd = 0

    # edge case: if diameter == 0, ppd = 0
    c.eq.s  $f4, $f10       # float flag = 1 if cost == 0
    bc1t    _edgecase       # jump to set ppd = 0

    # calculate ppd = PI*d^2/4/cost
    l.s     $f9, PI         # load PI into f9
    li.s    $f8, 4.0        # load 4.0 into f8
    mul.s   $f4, $f4, $f4   # f1 = diameter^2
    mul.s   $f4, $f9, $f4   # f1 = PI*diameter^2
    div.s   $f4, $f4, $f8   # f1 = PI*diameter^2/4
    div.s   $f4, $f4, $f5   # f1 = PI*diameter^2/4/cost, ppd is in f1

    # #DEBUG: PRINT THE FLOAT
    # #----------------------------
    # li      $v0, 2          # print float
    # mov.s     $f12, $f4         
    # syscall

    # # print new line
    # li      $v0, 4          # print string
    # la      $a0, str_newline  # load str_newline into a0
    # syscall
    # #----------------------------

    j       _save_ppd       # jump to save ppd in the heap, ignoring the edge case

    _edgecase:
        li.s    $f4, 0.0      # ppd = 0

    _save_ppd:
    # save ppd in heap
        s.s    $f4, 64($t4) # save ppd in heap

        # #DEBUG. PRINT FLOAT IN MEMORY
        # #----------------------------
        # li      $v0, 2          # print float
        # l.s     $f12, 64($t4)        
        # syscall

        # li      $v0, 4          # print string
        # la      $a0, str_newline  # load str_newline into a0
        # syscall
        # #----------------------------

# by this point in the code, everything should have been saved in heap
# except for t4 which is the start of this pizza in heap and
# t9 which is the head of the linked list

# we should be able to repurpose any t-registers except for t4, t9

# the heap consists of 64 bytes of chars, 4 bytes of float, and 4 bytes for the next pointer
# at this point the pointer will be empty

_insertion_sort:
    # insert this pizza into the sorted position on the list
    # t4: start of this pizza in heap (this)
    # t8: the current node when traversing the linked list (cur)
    # t9: the head of the linkedlist (head)

    # check if head is null
    beqz     $t9, _set_head   # if t9 == NULL then jump to set current pizza to head
    
    # check if this pizza is greater than the head
    move    $a0, $t9        # a0 = t9 = head
    move    $a1, $t4        # a1 = t4 = this pizza

    addi    $sp, $sp, -4      # open stack frame (1 word)
    sw      $ra, 0($sp)      # save ra
    
    jal     pizza_compare   # compare t9 and t4, outputs the greater pizza

    lw      $ra, 0($sp)      # restore ra
    addi    $sp, $sp, 4      # collapse frame (1 word)

    beq     $t4, $v0, _set_head     # if this pizza is greater than the head then set this pizza as the head

    move    $t8, $t9        # set current of the linkedlist = head

    _traverse_linked_list:
        # traverse the linkedlist and check to see where this pizza fits
        
        # check to see if the next after current is null
        lw      $t0, 68($t8)    # t0 = 68($t4)
        beqz    $t0,  _insert_node    # if nothing exists after current then insert this pizza

        # compare this pizza with cur -> next, if this pizza is greater then we will insert this pizza 
        lw      $t0, 68($t8)    # t0 = 68($t8)

        # # DEBUG - print cur -> next and this
        # # ----------------------------
        # li      $v0, 4          # print string
        # la      $a0, debug_str_a0  # load "a0" into a0
        # syscall

        # # print space
        # li      $v0, 4          # print string
        # la      $a0, str_space  # load str_space into a0
        # syscall
        
        # # print cur -> next
        # li      $v0, 4
        # move    $a0, $t0
        # syscall

        # # print space
        # li      $v0, 4          # print string
        # la      $a0, str_space  # load str_space into a0
        # syscall

        # li      $v0, 4          # print string
        # la      $a0, debug_str_a1  # load "a1" into a0
        # syscall

        # # print space
        # li      $v0, 4          # print string
        # la      $a0, str_space  # load str_space into a0
        # syscall

        # li      $v0, 4
        # move    $a0, $t4
        # syscall

        # # print space
        # li      $v0, 4          # print string
        # la      $a0, str_space  # load str_space into a0
        # syscall

        # # print new line
        # li      $v0, 4          # print string
        # la      $a0, str_newline  # load str_newline into a0
        # syscall
        # # ----------------------------

        move    $a0, $t0        # a0 = cur -> next
        move    $a1, $t4        # a1 = this pizza

        addi    $sp, $sp, -4    # open stack frame (1 word)
        sw      $ra, 0($sp)     # save ra

        jal     pizza_compare   # compare pointer of current pizza and the value of this pizza, outputs the greater pizza

        lw      $ra, 0($sp)     # restore ra
        addi    $sp, $sp, 4     # collapse frame (1 word)

        beq     $t4, $v0, _insert_node    # if this pizza is greater than cur->next then insert this pizza

        # all conditions have failed to exit loop, so we traverse to the next node in the list
        lw      $t8, 68($t8)    # t8 = 68($t8); cur = cur -> next
        j       _traverse_linked_list   # continue traversing the linked list

    _insert_node:
        # set this->next to cur->next and cur->next to this
        lw      $t0, 68($t8)    # t0 = cur -> next
        sw      $t0, 68($t4)    # this -> next = cur -> next
        sw      $t4, 68($t8)    # cur -> next = this

        # go to next pizza
        j       _read_input     # jump back to reading a new pizza

    _set_head:
        # set the pointer of this pizza to the previous head
        sw      $t9, 68($t4)    # pointer section of this pizza's heap is set to t9

        # set the current pizza as head
        move    $t9, $t4        # t9 = t4

        # go to next pizza
        j       _read_input     # jump back to reading a new pizza

_exit:
    # print everything and then end the program
    # t9: head

    # la      $t1, str_space
    # sb      $t1, 0($t5)             # set the \n to space

    _traverse_and_print:
    # traverse the linkedlist until it is null and print the contents
        
        # if head = null, then stop
        beqz    $t9, _end      # jump to end if t9 = 0

        # print head's name
        li      $v0, 4         # print string
        move    $a0, $t9       # move string section of head to a0
        syscall

        # print space
        li      $v0, 4          # print string
        la      $a0, str_space  # load str_space into a0
        syscall

        # print head's ppd
        li      $v0, 2          # print float
        l.s     $f12, 64($t9)   # load ppd section of head to f12     
        syscall

        # print new line
        li      $v0, 4          # print string
        la      $a0, str_newline  # load str_newline into a0
        syscall

        # check to see if the next after current is null
        lw      $t0, 68($t9)     # load the pointer of the head to t0
        beq     $t0, $0, _end    # if nothing exists after current then end the program

        # we traverse to the next node in the list
        lw      $t9, 68($t9)      # head = head -> next
        j       _traverse_and_print   # continue traversing the linked list

    _end:
        jr      $ra


pizza_compare:
    # compare two pizzas stored in a0 and a1 and determine which one is greater
    # a0, a1 are addresses of the starting positions of those pizzas on the heap
    # returns the starting address of the greater pizza in v0
    # this function only uses s registers
    
    addi    $sp, $sp, -20    # open frame (5 words)
    s.s     $f21, 16($sp)    # save f21
    s.s     $f20, 12($sp)    # save f20
    sw      $s5, 8($sp)      # save s5
    sw      $s1, 4($sp)      # save s1
    sw      $s0, 0($sp)      # save s0

    move    $s0, $a0         # s0 = address of pizza0
    move    $s1, $a1         # s1 = address of pizza1

    # compare ppd
    l.s     $f20, 64($s0)    # f20 = ppd of pizza0
    l.s     $f21, 64($s1)    # f21 = ppd of pizza1

    c.eq.s  $f20, $f21       # sets float flag to true if f20 = f21

    # if f20 = f21 (ppd are equal) then compare the strings of the two pizzas
    move    $a0, $s0         # a0 = s0
    move    $a1, $s1         # a1 = s1
    bc1t    _compare_names   # jump to compare the strings alphabetically

    # this section only applies if ppd is not equal
    c.lt.s  $f20, $f21       # float flag = 1 if f20 < f21, otherwise float flag = 0
    bc1t    _pizza1_greater  # if f20 < f21 then jump to pizza0_greater

_pizza0_greater:
    # this section only applies if pizza 0 is greater
    # return pizza 0 and close stack frame

    move    $v0, $s0         # move address of pizz1 to v0

    lw      $s0, 0($sp)      # restore s0
    lw      $s1, 4($sp)      # restore s1
    lw      $s5, 8($sp)      # restore s5
    l.s     $f20, 12($sp)    # restore f20
    l.s     $f21, 16($sp)    # restore f21
    addi    $sp, $sp, 20     # collapse frame (5 words)

    jr      $ra              # return


_pizza1_greater:
    # return pizza 1 and close stack frame
    move    $v0, $s1         # move address of pizz1 to v0

    lw      $s0, 0($sp)      # restore s0
    lw      $s1, 4($sp)      # restore s1
    lw      $s5, 8($sp)      # restore s5
    l.s      $f20, 12($sp)    # restore f20
    l.s      $f21, 16($sp)    # restore f21
    addi    $sp, $sp, 20     # collapse frame (5 words)

    jr      $ra              # return

_compare_names:
    # we are given that if two pizza ppds are equal, their names must be different

    # save ra here so that we don't get caught in an infinite loop
    addi    $sp, $sp, -4      # open stack frame (1 word)
    sw      $ra, 0($sp)      # save ra

    jal     str_compare      # jump to str_compare function, outputs in v0: 1 if a0 > a1 and -1 if a0 < a1
    
    lw      $ra, 0($sp)      # restore ra
    addi    $sp, $sp, 4      # collapse frame (1 word)


    li      $s5, 1           # s5 = 1

    beq		$v0, $s5, _pizza1_greater   # if $v0 == 1 then a0 > a1 so jump to _pizza1_greater
    j		_pizza0_greater				# if above does not occur then pizza0 must be greater


str_compare:
    # compare two strings, given that they are different
    # inputs: a0, a1 are addresses of strings
    # outputs: if a0 < a1, -1 ; if a0 > a1, 1

    addi    $sp, $sp, -24    # open frame (6 words)
    sw      $s8, 20($sp)     # save s8
    sw      $s7, 16($sp)     # save s7
    sw      $s6, 12($sp)     # save s6
    sw      $s5, 8($sp)      # save s5
    sw      $s1, 4($sp)      # save s1
    sw      $s0, 0($sp)      # save s0

    move    $s0, $a0         # s0 = address of str0
    move    $s1, $a1         # s1 = address of str1

_loop_chars:
    # iterate through each character in the string
    lb      $s5, 0($s0)      # Read one byte from str0 into s5
    lb      $s6, 0($s1)      # Read one byte from str1 into s6
    addi    $s0, $s0, 1      # increment str0 to the next char
    addi    $s0, $s1, 1      # increment str1 to the next char

    beq     $s5, $s6, _loop_chars       # if the current bytes are equal then go to the next set of chars
    
    # this section only occurs if the current bytes are not equal
    # if char 0 != char 1, output 1 if a0 > a1, output -1 if a0 < a1
    slt		$s8, $s5, $s6	 # s8 = (s5 < s6) ? 1 : 0
    beqz    $s8, _a1_smaller # if s5 > s6 then return 1 and close frame
    j       _a0_smaller      # else, then return 0 and close frame

_a1_smaller:
    # return 1 and close stack frame
    li      $v0, 1           # v0 = 1, since a0 > a1

    lw      $s0, 0($sp)      # restore s0
    lw      $s1, 4($sp)      # restore s1
    lw      $s5, 8($sp)      # restore s5
    lw      $s6, 12($sp)     # restore s6
    lw      $s7, 16($sp)     # restore s7
    lw      $s8, 20($sp)     # restore s8
    addi    $sp, $sp, 24     # collapse frame (6 words)

    jr      $ra              # return

_a0_smaller:
    # return -1 and close stack frame
    li      $v0, -1           # v0 = -1, since a0 < a1

    lw      $s0, 0($sp)      # restore s0
    lw      $s1, 4($sp)      # restore s1
    lw      $s5, 8($sp)      # restore s5
    lw      $s6, 12($sp)     # restore s6
    lw      $s7, 16($sp)     # restore s7
    lw      $s8, 20($sp)     # restore s8
    addi    $sp, $sp, 24     # collapse frame (6 words)

    jr      $ra              # return


.data
buffer: .space 64
strDONE: .asciiz "DONE"
prompt_name: .asciiz "Pizza name: \t"
prompt_diameter: .asciiz "Pizza diameter: "
prompt_cost: .asciiz "Pizza cost: \t"
str_newline: .asciiz "\n"
str_space: .asciiz " "
debug_str_a0: .asciiz "cur->next"
debug_str_a1: .asciiz "this"
PI: .float 3.14159265358979323846