org #1000

1000			ld a,#0 			3E00
1002			ld b,#f				060F
1004 	loop:	add a,a				87
1005        	ld d,a				57
1006			djnz loop			10FC
1008			ld a,#0				3E00
100A	loop3:  halt				76

end
