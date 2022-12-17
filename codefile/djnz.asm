org #1000

		ld a,#1 
		ld b,#7
loop:	add a,a
        ld d,a
		djnz loop
		ld a,#0
loop3:  halt

end
