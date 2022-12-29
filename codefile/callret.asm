org #1200

loop:	ld a,#1
		ld b,#ef
		add a,b
		jr z,loop

loop2:	ld a,b
		call subrtn
		add a,#3
		halt

subrtn:	ld a,#2
		ld b,#cd
		ret
end
