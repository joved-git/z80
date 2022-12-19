org #1300

1300	init:		ld a,#FF		3EFF	
1302				ld b,#FC		06FC
1304	loop:		add a,#1		C601
1306				nop				00	
1307				jr c,loop		38FB

1309				ld a,#FD		3EFD
130B	cploop:		cp b			B8
130C				jr z,next		28xx
130E				inc b			04
130F				jr cploop		18FB

1311	next:		ld b,#10		0610
1313				ld a,#E0		3EE0
1315	nocy:		add a,b			80
1316				jr nc,nocy		30xx
1318				ld a,#F1		3EF1
131A				ld h,#EF		26EF
131C	reloop:		dec a			3D
131D				cp h			BC
131E				jr nz,reloop	20xx
1320				halt			76

end
