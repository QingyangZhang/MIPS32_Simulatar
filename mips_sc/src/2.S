#include "trap.h"
   .set noat
   .globl main
   .text
main:
    li $s0, 0x10000001           # $s0=0x10000001
    li $s1, 0x10000010           # $s1=0x10000010
    li $s3, 0x00000020           # $s3=0x00100000  32
    nop
    move $a0, $zero              # $a0=0 base
    li $s4, 0xfffffff0           # $s4=0xfffffff0
    nop
    sub $t0, $s0, $s1           # $t0=$s0-$s1   overflow
    subu $t1, $s0, $s1          # $t0=$s0-$s1 unsigned
    sllv $t2, $s4, $s3          # $t2=$t4<<$t3
    addi $s5, $s4, 0x0001   # $s5=s4+1
    sll  $t3, $s5, 0x0010   # $t3=$s5<<16
    srl  $a2, $s4, 0x0010   # $a2=$s4>>16
    srlv $a3, $s5, $a3          # $a3=$s5>>$t3
    sw  $s1, 8($a0)             # (8)=$s1
    sw  $t2, 4($a0)             # (4)=$t2
    li $t4, 0x11111111          # $t4=0x11111111
    li $t5, 0x11111110          # $t5=0x11111110
    lw $s7, 4($a0)              # $s7=(4)
    lw $s0, 8($a0)              # $s0=(8)
    sltu $a1, $t4, $t5          # $a1=(t4<t5)?1:0  0

    HIT_GOOD_TRAP		#stop temu