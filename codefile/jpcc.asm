org #0480

init:	ld a,#FE
loop:	add a,#1
		nop
		nop
		jp c,loop

		ld a,#FC
		jp init
		nop
		halt
end
