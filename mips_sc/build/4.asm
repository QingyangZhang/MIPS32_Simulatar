
build/4:     file format elf32-tradlittlemips
build/4


Disassembly of section .text:

00000000 <main>:
   0:	3c101234 	lui	s0,0x1234
   4:	3c11f234 	lui	s1,0xf234
   8:	36315678 	ori	s1,s1,0x5678
   c:	24130010 	li	s3,16
  10:	00002021 	move	a0,zero
  14:	00000000 	nop
  18:	0010a3c3 	sra	s4,s0,0xf
  1c:	0270a807 	srav	s5,s0,s3
  20:	02340018 	mult	s1,s4
  24:	2e281000 	sltiu	t0,s1,4096
  28:	3416f123 	li	s6,0xf123
  2c:	00000000 	nop
  30:	00004810 	mfhi	t1
  34:	00005012 	mflo	t2
  38:	0236582a 	slt	t3,s1,s6
  3c:	00000000 	nop
  40:	02350019 	multu	s1,s5
  44:	ac890008 	sw	t1,8(a0)
  48:	a48a0008 	sh	t2,8(a0)
  4c:	00000000 	nop
  50:	00004810 	mfhi	t1
  54:	00005012 	mflo	t2
  58:	00000000 	nop
  5c:	02200011 	mthi	s1
  60:	02400013 	mtlo	s2
  64:	4a000000 	c2	0x0

Disassembly of section .reginfo:

00000000 <.reginfo>:
   0:	007f0f10 	0x7f0f10
	...
