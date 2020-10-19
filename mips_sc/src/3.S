#include "trap.h"
   .set noat
   .globl main
   .text
main:
    li $s0, 0xffffffff          # $s0=0xffffffff
    li $s1, 0xf0000000          # $s1=0xf0000000
    move $a0, $zero             # $a0=0 base
    move $s2, $s0		        # $s2 = $s0
    move $s3, $s1               # $a3=$a1
    and  $t1, $s0, $s1          # $t1=$s0 & $s1
    or   $t2, $s0, $s1          # $t2=s0 | s1
    xor  $t3, $s0, $s2          # $t3=s0 xor s2
    sb   $t1, 64($a0)           # (64)_b=$t1
    sh   $t2, 64($a0)           # (64)_h=$t2
    sb   $t1, 128($a0)          # (128)=$t1
    andi $t1, $s0, 0x1111   # $t1=$s0 & 0x11110000
    andi $t2, $s1, 0x0011   # $t2=$s1 & 0x00001111
    xori $t4, $s0, 0xfff0   # $t4=$s0 xor 0xfffffff0
    lb   $t5, 64($a0)           # $t5=(64)_b
    lbu  $t6, 64($a0)           # $t6=(64)_bu
    lh   $t7, 128($a0)          # $t7=(128)_h
    lhu  $a1, 128($a0)          # $a1=(128)_hu
    lw   $a2, 0($a0)            # $a2=(0)
    nor  $a3, $s0,$s0           # $a3=$s0 nor $s0
    nor  $s4, $s0,$s1

    HIT_GOOD_TRAP		#stop temu