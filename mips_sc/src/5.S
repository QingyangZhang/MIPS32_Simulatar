#include "trap.h"
   .set noat
   .globl main
   .text
main:
    li $s0, 0xffffffff              # $s0=0xffffffff
    li $s1, 0x00000014              # $s1=0x00000014
    move $a0, $zero                 # $a0=$zero
    nop
    srl $s2, $s0, 0x001f            # $s2=$s0>>31
    nop
    nop
    add $s0, $s1, $s2               # $s0=$s1+$s2
    nop
    li $s4, 0xffffffff              # $s4=0xffffffff
    sll $s1, $s0, 0x0010            # $s1=$s0<<16
    mult $s1, $s4                   # $s1*$s4
    nop
    addi $s4, $s1, 0x1fff           # $s4=$s1+0x1fff
    add $s5, $s0, $s2               # $a5=$a0+$a2
    nop
    sra $t0,$s0, 0x0002             # $t0=$s0>>2
    srl $t1, $s5, 0x000f            # $t1=$s5>>15
    nop
    nop
    sb $t0, 208($a0)                  # (8)_b=$t0
    mtlo $t1                        # $lo=$t1
    nop
    nop
    slt $t3, $t1, $s0               # $t3=($t1<$s0)?1:0
    multu $s0, $t0                  # $s0*$t0 unsign
    HIT_GOOD_TRAP		#stop temu