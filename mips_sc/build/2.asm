
build/2:     file format elf32-tradlittlemips
build/2


Disassembly of section .text:

00000000 <main>:
   0:	3c101000 	lui	s0,0x1000
   4:	36100001 	ori	s0,s0,0x1
   8:	3c111000 	lui	s1,0x1000
   c:	36310010 	ori	s1,s1,0x10
  10:	24130020 	li	s3,32
  14:	00000000 	nop
  18:	00002021 	move	a0,zero
  1c:	2414fff0 	li	s4,-16
  20:	00000000 	nop
  24:	02114022 	sub	t0,s0,s1
  28:	02114823 	subu	t1,s0,s1
  2c:	02745004 	sllv	t2,s4,s3
  30:	22950001 	addi	s5,s4,1
  34:	00155c00 	sll	t3,s5,0x10
  38:	00143402 	srl	a2,s4,0x10
  3c:	00f53806 	srlv	a3,s5,a3
  40:	ac910008 	sw	s1,8(a0)
  44:	ac8a0004 	sw	t2,4(a0)
  48:	3c0c1111 	lui	t4,0x1111
  4c:	358c1111 	ori	t4,t4,0x1111
  50:	3c0d1111 	lui	t5,0x1111
  54:	35ad1110 	ori	t5,t5,0x1110
  58:	8c970004 	lw	s7,4(a0)
  5c:	8c900008 	lw	s0,8(a0)
  60:	018d282b 	sltu	a1,t4,t5
  64:	4a000000 	c2	0x0

Disassembly of section .reginfo:

00000000 <.reginfo>:
   0:	00bb3ff0 	0xbb3ff0
	...
