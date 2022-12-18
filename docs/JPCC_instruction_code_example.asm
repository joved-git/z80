org #0480

0480	init:	ld a,#FE		3EFE	
0482	loop:	add a,#1		C601
0484			nop				00
0485			nop				00
0486			jp nc,loop		D28204

0489			ld a,#FC		3EFC
048B			jp init			C38004
048E			nop				00
048F			halt			76

end
