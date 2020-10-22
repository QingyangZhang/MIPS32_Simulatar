
build/5:     file format elf32-tradlittlemips
build/5


Disassembly of section .text:

00000000 <main>:
   0:	2410ffff 	li	s0,-1
   4:	24110014 	li	s1,20
   8:	00002021 	move	a0,zero
   c:	00000000 	nop
  10:	001097c2 	srl	s2,s0,0x1f
	...
  1c:	02328020 	add	s0,s1,s2
  20:	00000000 	nop
  24:	2414ffff 	li	s4,-1
  28:	00108c00 	sll	s1,s0,0x10
  2c:	02340018 	mult	s1,s4
  30:	00000000 	nop
  34:	22341fff 	addi	s4,s1,8191
  38:	0212a820 	add	s5,s0,s2
  3c:	00000000 	nop
  40:	00104083 	sra	t0,s0,0x2
  44:	00154bc2 	srl	t1,s5,0xf
	...
  50:	a0880008 	sb	t0,8(a0)
  54:	01200013 	mtlo	t1
	...
  60:	0130582a 	slt	t3,t1,s0
  64:	02080019 	multu	s0,t0
  68:	4a000000 	c2	0x0

Disassembly of section .reginfo:

00000000 <.reginfo>:
   0:	00370b10 	0x370b10
	...
