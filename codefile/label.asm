org #1234

loop:	
		ld a,#1
		ld b,#CD
		add a,b
		jump loop

loop2:	
		ld a,b
		jump loop2
end
