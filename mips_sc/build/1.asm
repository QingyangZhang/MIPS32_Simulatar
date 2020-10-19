
build/1:     file format elf32-tradlittlemips
build/1


Disassembly of section .text:

00000000 <main>:
   0:	24081011 	li	t0,4113
   4:	24090010 	li	t1,16
   8:	00002021 	move	a0,zero
	...
  18:	01098820 	add	s1,t0,t1
	...
  24:	22320110 	addi	s2,s1,272
	...
  30:	02409821 	move	s3,s2
  34:	26520111 	addiu	s2,s2,273
	...
  40:	02722822 	sub	a1,s3,s2
	...
  4c:	20a61001 	addi	a2,a1,4097
	...
  58:	00c5582a 	slt	t3,a2,a1
  5c:	0000a021 	move	s4,zero
	...
  68:	ac8b0004 	sw	t3,4(a0)
  6c:	2a8d0011 	slti	t5,s4,17
  70:	ac8d0008 	sw	t5,8(a0)
  74:	3c171111 	lui	s7,0x1111
  78:	36f71111 	ori	s7,s7,0x1111
  7c:	0017702b 	sltu	t6,zero,s7
  80:	ac8e0018 	sw	t6,24(a0)
  84:	4a000000 	c2	0x0

Disassembly of section .reginfo:

00000000 <.reginfo>:
   0:	009e6b70 	0x9e6b70
	...
