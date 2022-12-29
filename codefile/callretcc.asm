org #1200

	    ld a,#f0
		ld b,#ef

		call subrtn
		ld a,#0
		halt

subrtn:	
loop:   cp b
		ret z
        inc B
        jr loop
		
end