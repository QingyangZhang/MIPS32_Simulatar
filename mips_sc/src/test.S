#include "trap.h"
   .set noat
   .globl main
   .text
main:
   li $t0, 0x00001011            # $t0=0x00001011
   li $t1, 0x00000010            # $t1=0x00000010
   nop
   nop
   nop
   add $s1, $t0, $t1             # $s1=$t0+t1
   nop
   nop
   addi $s2, $s1, 0x1010     # $s2=$s1+0x01111010
   nop
   nop
   move $s3,$s2                  # $s3=$s2
   addiu $s2,$s2, 0x1111     # $s2=$s2+0x01111111  unsigned
   nop
   nop
   sub $a1, $s3, $s2             # $a1=$s3-$s2
   nop
   nop
   addi $a2, $a1, 0x0001     # $a2=$a1+0x10000001
   nop
   nop
   slt  $t3, $a2, $a1            # $t3=($a2<$a1)?1:0
   move $s4, $zero               # $s4=0
   nop
   nop
#   li $t4, 0x10010000            # mars
   sw $t3, 4($zero)                # (4)=$t3
   slti $t5, $s4,0x1111      # $t5=($t4<0x01111111)?1:0
   sw $t5, 8($zero)                # (8)=$t5
   li $s7, 0x11111111            # $s7=0x111111111
   sltu $t6, $zero, $s7          # $t6=(0<0x11111111)?1:0  1
   sw $t6, 24($zero)               # (24)=$t6

   HIT_GOOD_TRAP		#stop temu
