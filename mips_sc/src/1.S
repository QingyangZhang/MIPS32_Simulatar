#include "trap.h"
   .set noat
   .globl main
   .text
main:
   li $t0, 0x00001011            # $t0=0x00001011
   li $t1, 0x00000010            # $t1=0x00000010
   move $a0, $zero
   nop
   nop
   nop
   add $s1, $t0, $t1             # $s1=$t0+t1
   nop
   nop
   addi $s2, $s1, 0x0110     # $s2=$s1+0x0110  
   nop
   nop
   move $s3,$s2                  # $s3=$s2
   addiu $s2,$s2, 0x0111     # $s2=$s2+0x0111  unsigned
   nop
   nop
   sub $a1, $s3, $s2             # $a1=$s3-$s2
   nop
   nop
   addi $a2, $a1, 0x1001     # $a2=$a1+0x1001
   nop
   nop
   slt  $t3, $a2, $a1            # $t3=($a2<$a1)?1:0
   move $s4, $zero               # $s4=0
   nop
   nop
   sw $t3, 4($a0)                # (4)=$t3
   slti $t5, $s4,0x0011      # $t5=($t4<0x0011)?1:0
   sw $t5, 8($a0)                # (8)=$t5
   li $s7, 0x11111111            # $s7=0x111111111
   sltu $t6, $zero, $s7          # $t6=(0<0x11111111)?1:0  1
   sw $t6, 24($a0)               # (24)=$t6

   HIT_GOOD_TRAP		#stop temu
