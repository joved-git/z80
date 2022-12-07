org #1234

loop:	
		ld a,#1
		ld b,#CD
		add a,b
		jp loop

loop2:	
		ld a,b
		jp loop2
end
