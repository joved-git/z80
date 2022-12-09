org #0480

loop:	ld a,#1
		ld b,#CD
		add a,b
		jp nz,loop

loop2:	ld a,b
		call p,loop3
		nop
		nop

loop3:	ld a,#2
		halt
end
