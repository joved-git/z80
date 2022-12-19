org #1300

init:	ld a,#FF
		ld b,#FC
loop:	add a,#1
		nop
		jr c,loop
		ld a,#FD
cploop:	cp b
		jr z,next
		inc b
		jr cploop
next:	ld b,#10
		ld a,#E0
nocy:	add a,b
		jr nc,nocy
		ld a,#F1
		ld h,#EF
reloop:	dec a
		cp h
		jr nz,reloop
		halt
end
