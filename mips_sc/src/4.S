#include "trap.h"
   .set noat
   .globl main
   .text
main:
    li $s0, 0x12340000              # $s0=0x12340000
    li $s1, 0xf2345678              # $s1=0xf2345678
    li $s3, 0x00000010              # $s3=0x00000010
    move $a0, $zero                 # $a0=$zero base
    nop 
    sra $s4, $s0, 0x000f        # $s4=$s0>>15
    srav $s5, $s0, $s3              # $s5=$s0>>$a3
    mult $s1, $s4                   # $s1*$s4
    sltiu $t0, $s1, 0x1000      # $t0=($s1<0x00001000)?1:0
    li $s6, 0xf123              # $s6=0xf1234567
    nop     
    mfhi $t1                        # $t1=$hi
    mflo $t2                        # $t2=$lo
    slt $t3, $s1, $s6               # $t3=($s1<$s5)?1:0
    multu $s1, $s5                  # $1*$s5 unsign
    sw $t1, 8($a0)                  # (8)=$t1
    sh $t2, 8($a0)                  # (8)_h=$t2
    nop
    mfhi $t1                        # $t1=$hi
    mflo $t2                        # $t2=$lo
    mthi $s1                        # $hi=$s1
    mtlo $s2                        # $lo=$s2

    HIT_GOOD_TRAP		#stop temu