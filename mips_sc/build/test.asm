
build/test:     file format elf32-tradlittlemips
build/test


Disassembly of section .text:

00000000 <main>:
   0:	24081011 	li	t0,4113
   4:	24090010 	li	t1,16
	...
  14:	01098820 	add	s1,t0,t1
	...
  20:	22321010 	addi	s2,s1,4112
	...
  2c:	02409821 	move	s3,s2
  30:	26521111 	addiu	s2,s2,4369
	...
  3c:	02722822 	sub	a1,s3,s2
	...
  48:	20a60001 	addi	a2,a1,1
	...
  54:	00c5582a 	slt	t3,a2,a1
  58:	0000a021 	move	s4,zero
	...
  64:	ac0b0004 	sw	t3,4(zero)
  68:	2a8d1111 	slti	t5,s4,4369
  6c:	ac0d0008 	sw	t5,8(zero)
  70:	3c171111 	lui	s7,0x1111
  74:	36f71111 	ori	s7,s7,0x1111
  78:	0017702b 	sltu	t6,zero,s7
  7c:	ac0e0018 	sw	t6,24(zero)
  80:	4a000000 	c2	0x0

Disassembly of section .reginfo:

00000000 <.reginfo>:
   0:	009e6b60 	0x9e6b60
	...
