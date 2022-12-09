org #1234

loop:	ld a,#1
        ld b,#CD
        add a,b
        jr loop
        ld a,b
	 	jr loop2
        ld b,c
loop2:  ld hl,#3412
loop3:  halt

end
