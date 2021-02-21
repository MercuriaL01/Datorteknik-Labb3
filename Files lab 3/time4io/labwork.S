  # labwork.S
  # Written 2015-2017 by F Lundevall
  # Skeleton file for IS1200/IS1500 lab 1.
  # The macros PUSH and POP are in the public domain.
  # Please add your own code at the end of the file.

  #
  # Please keep the two macros PUSH and POP unchanged
  #
.macro	PUSH reg
	addi	$sp,$sp,-4
	sw	\reg,0($sp)
.endm

.macro	POP reg
	lw	\reg,0($sp)
	addi	$sp,$sp,4
.endm

.global
.data
.align 2
mytime:	.word 0x5957
timstr:	.ascii "text more text lots of text\0"
.text
main:
# print timstr
la	$a0,timstr
li	$v0,4
syscall
nop
# wait a little
li	$a0,2
jal	delay
nop
# call tick
la	$a0,mytime
jal	tick
nop
# call your function time2string
la	$a0,timstr
la	$t0,mytime
lw	$a1,0($t0)
jal	time2string
nop
# print a newline
li	$a0,10
li	$v0,11
syscall
nop
# go back and do it all again
j	main
nop
# tick: update time pointed to by $a0
tick:	lw	$t0,0($a0)	# get time
addiu	$t0,$t0,1	# increase
andi	$t1,$t0,0xf	# check lowest digit
sltiu	$t2,$t1,0xa	# if digit < a, okay
bnez	$t2,tiend
nop
addiu	$t0,$t0,0x6	# adjust lowest digit
andi	$t1,$t0,0xf0	# check next digit
sltiu	$t2,$t1,0x60	# if digit < 6, okay
bnez	$t2,tiend
nop
addiu	$t0,$t0,0xa0	# adjust digit
andi	$t1,$t0,0xf00	# check minute digit
sltiu	$t2,$t1,0xa00	# if digit < a, okay
bnez	$t2,tiend
nop
addiu	$t0,$t0,0x600	# adjust digit
andi	$t1,$t0,0xf000	# check last digit
sltiu	$t2,$t1,0x6000	# if digit < 6, okay
bnez	$t2,tiend
nop
addiu	$t0,$t0,0xa000	# adjust last digit
tiend:	sw	$t0,0($a0)	# save updated result
jr	$ra		# return
nop

# you can write your code for subroutine "hexasc" below this line
#
  #
  # Please add your own code below this line
  #
  .global hexasc
  hexasc:
  	andi	$t0,$a0,0xf	#för att få 4 LSB från $a0

  	ble	$t0,9,ascNumbers
  	nop			#har man efter "metod anrop"

  	ble	$t0,15,ascLetters
  	nop			#har man efter "metod anrop"

  ascNumbers:
  	addi $v0,$t0,0x30	#
  	jr $ra
  	nop

  ascLetters:
  	addi $v0,$t0,0x37	#
  	jr $ra
  	nop

.global delay
  delay:
	PUSH $ra
	li $t2, 0 			# int i;
	li $t3, 4711			# i < (det jag sätter t3 till)

	while:
		ble $a0, 0, finish	# branch less or equal for 0 and t1
		nop			# nop efter branch
		sub $a0, $a0, 1		# ms = ms - 1;

		for:
			beq $t2, $t3, while	# t2 är i och t3 är max i får vara
			nop			# nop efter branch
			addi $t2, $t2, 1	# i = i + 1
			j for			# börja om for loopen
			nop			# nop efter jag callar en "metod"

finish:
	POP $ra
	jr $ra
	nop


.global time2string
  time2string:
	PUSH $s1
	PUSH $ra
	move $s1,$a0

	# do intial comparison
	andi $t0,$a1,0xff		#Sätt de 8 LSB från a1 till t0

	andi $t0, $a1, 0xf000		#t0 sätts till fjärde LSB
	srl $a0,$t0,12			#a0 sätts till t0 fast shifted 12 right. Så att man får t.ex. 1111 0000 0000 0000 till 0000 0000 0000 1111    (allt i LSB)
	jal hexasc			#kör hexasc som konverterar till ASCII koder.
	nop				#Detta har man efter "metod" anrop
	move $a2,$v0			#sätter a2 till det v0 är. v0 kommer ifrån hexasc
	sb $a2,0($s1)			#sätter in a2 till MSB i s1

	andi $t0, $a1, 0x0f00
	srl $a0,$t0,8
	jal hexasc
	nop
	move $a2,$v0
	sb $a2,1($s1)

	li $t0, 0x3a    			#t0 sätts till colon
	sb $t0, 2($s1)			#detta sätts in i den tredje byten från vänster i s1

	andi $t0, $a1, 0x00f0
	srl $a0,$t0,4
	jal hexasc
	nop
	move $a2,$v0
	sb $a2,3($s1)

	andi $t0, $a1, 0x000f
	srl $a0,$t0,0
	jal hexasc
	nop
	move $a2,$v0
	sb $a2,4($s1)

	li $t0, 0x0			#t0 sätts till null
	sb $t0,5($s1)			#LSB sätts tll null byten

	end:
		POP $ra
		POP $s1

	jr $ra				#return
	nop				#detta har man efter hopp och metod grejer tror jag

	j end  				#få originella värderna på ra och s1
	nop
