 #
 # 11 Dec 96 - eay - added bn_div64 (or should that be bn_div128 :-)
 # 20 Dec 95 - eay - This is quick hackup of the routines found in bn_mulw.c
 #
	.verstamp 3 11 2 7 0
	.set noreorder
	.set volatile
	.set noat
	.file	1 "alpha2.s"
gcc2_compiled.:
__gnu_compiled_c:
.text
	.align 3
	.globl bn_mul_add_word
	.ent bn_mul_add_word
bn_mul_add_word:
bn_mul_add_word..ng:
	.frame $30,0,$26,0
	.prologue 0
	subq $18,2,$25	# num=-2
	bis $31,$31,$0
	blt $25,$42
	.align 5
$142:
	subq $18,2,$18	# num-=2
	subq $25,2,$25	# num-=2

	ldq $1,0($17)	# a[0]
	ldq $2,8($17)	# a[1]

	mulq $19,$1,$3	# a[0]*w low part	r3
 	umulh $19,$1,$1 # a[0]*w high part	r1
	mulq $19,$2,$4	# a[1]*w low part	r4
 	umulh $19,$2,$2 # a[1]*w high part	r2

	ldq $22,0($16)	# r[0]			r22
	ldq $23,8($16)	# r[1]			r23

	addq $3,$22,$3	# a0 low part + r[0]	
	addq $4,$23,$4	# a1 low part + r[1]	
	cmpult $3,$22,$5 # overflow?
	cmpult $4,$23,$6 # overflow?
	addq $5,$1,$1	# high part + overflow 
	addq $6,$2,$2	# high part + overflow 

	addq $3,$0,$3	# add c
	cmpult $3,$0,$5 # overflow?
	stq $3,0($16)
	addq $5,$1,$0	# c=high part + overflow 

	addq $4,$0,$4	# add c
	cmpult $4,$0,$5 # overflow?
	stq $4,8($16)
	addq $5,$2,$0	# c=high part + overflow 

	ble $18,$43

 	addq $16,16,$16
 	addq $17,16,$17
	blt $25,$42

 	br $31,$142
$42:
	ldq $1,0($17)	# a[0]
 	umulh $19,$1,$3 # a[0]*w high part
	mulq $19,$1,$1	# a[0]*w low part
	ldq $2,0($16)	# r[0]
	addq $1,$2,$1	# low part + r[0]
	cmpult $1,$2,$4 # overflow?
	addq $4,$3,$3	# high part + overflow 
	addq $1,$0,$1	# add c
	cmpult $1,$0,$4 # overflow?
	addq $4,$3,$0	# c=high part + overflow 
	stq $1,0($16)

	.align 4
$43:
	ret $31,($26),1
	.end bn_mul_add_word
	.align 3
	.globl bn_mul_word
	.ent bn_mul_word
bn_mul_word:
bn_mul_word..ng:
	.frame $30,0,$26,0
	.prologue 0
	subq $18,2,$25	# num=-2
	bis $31,$31,$0
	blt $25,$242
	.align 5
$342:
	subq $18,2,$18	# num-=2
	subq $25,2,$25	# num-=2

	ldq $1,0($17)	# a[0]
	ldq $2,8($17)	# a[1]

	mulq $19,$1,$3	# a[0]*w low part	r3
 	umulh $19,$1,$1 # a[0]*w high part	r1
	mulq $19,$2,$4	# a[1]*w low part	r4
 	umulh $19,$2,$2 # a[1]*w high part	r2

	addq $3,$0,$3	# add c
	cmpult $3,$0,$5 # overflow?
	stq $3,0($16)
	addq $5,$1,$0	# c=high part + overflow 

	addq $4,$0,$4	# add c
	cmpult $4,$0,$5 # overflow?
	stq $4,8($16)
	addq $5,$2,$0	# c=high part + overflow 

	ble $18,$243

 	addq $16,16,$16
 	addq $17,16,$17
	blt $25,$242

 	br $31,$342
$242:
	ldq $1,0($17)	# a[0]
 	umulh $19,$1,$3 # a[0]*w high part
	mulq $19,$1,$1	# a[0]*w low part
	addq $1,$0,$1	# add c
	cmpult $1,$0,$4 # overflow?
	addq $4,$3,$0	# c=high part + overflow 
	stq $1,0($16)
$243:
	ret $31,($26),1
	.end bn_mul_word
	.align 3
	.globl bn_sqr_words
	.ent bn_sqr_words
bn_sqr_words:
bn_sqr_words..ng:
	.frame $30,0,$26,0
	.prologue 0
	
	subq $18,2,$25	# num=-2
	blt $25,$442
	.align 5
$542:
	subq $18,2,$18	# num-=2
	subq $25,2,$25	# num-=2

	ldq $1,0($17)	# a[0]
	ldq $4,8($17)	# a[1]

	mulq $1,$1,$2	# a[0]*w low part	r2
 	umulh $1,$1,$3 # a[0]*w high part	r3
	mulq $4,$4,$5	# a[1]*w low part	r5
 	umulh $4,$4,$6 # a[1]*w high part	r6

	stq $2,0($16)	# r[0]
	stq $3,8($16)	# r[1]
	stq $5,16($16)	# r[3]
	stq $6,24($16)	# r[4]

	ble $18,$443

 	addq $16,32,$16
 	addq $17,16,$17
	blt $25,$442
 	br $31,$542

$442:
	ldq $1,0($17)   # a[0]
	mulq $1,$1,$2   # a[0]*w low part       r2
        umulh $1,$1,$3  # a[0]*w high part       r3
	stq $2,0($16)   # r[0]
        stq $3,8($16)   # r[1]

	.align 4
$443:
	ret $31,($26),1
	.end bn_sqr_words

 #
 # What follows was taken directly from the C compiler with a few
 # hacks to redo the lables.
 #
	.extern	_iob 0
	.data	
	.align	3
	.align	0
$$21:
	.ascii	"Division would overflow\X0A\X00"
	.text	
	.align	4
	.file	2 "bn_mulw.c"
	.globl	bn_div64
 	.loc	2 225
 #  225		{
 	.ent	bn_div64 2
bn_div64:
 #	.option	O2
	ldgp	$gp, 0($27)
	lda	$sp, -144($sp)
	stq	$26, 0($sp)
	.mask	0x04000000, -144
	.frame	$sp, 144, $26, 48
	.prologue	1
	bis	$16, $16, $2
	bis	$17, $17, $21
	bis	$18, $18, $4
	.loc	2 225

	.loc	2 226
 #  226		BN_ULONG dh,dl,q,ret=0,th,tl,t;
	bis	$31, $31, $5
	.loc	2 227
 #  227		int i,count=2;
	ldil	$3, 2
	.loc	2 229
 #  228	
 #  229		if (d == 0) return(-1);
	bne	$4, $938
	.loc	2 229

	ldiq	$0, -1
	br	$31, $50
$938:
	.loc	2 231
 #  230	
 #  231		i=BN_num_bits_word(d);
	bis	$4, $4, $16
	stq	$21, 104($sp)
	stq	$2, 96($sp)
	stl	$3, 24($sp)
	stq	$4, 112($sp)
	stq	$5, 64($sp)
	.livereg	0x0001C002,0x00000000
	jsr	$26, BN_num_bits_word
	ldgp	$gp, 0($26)
	ldq	$21, 104($sp)
	ldq	$2, 96($sp)
	ldl	$3, 24($sp)
	ldq	$4, 112($sp)
	ldq	$5, 64($sp)
	bis	$0, $0, $18
	.loc	2 232
 #  232		if ((i != BN_BITS2) && (h > 1<<i))
	cmpeq	$0, 64, $6
	bne	$6, $939
	ldil	$7, 1
	sll	$7, $0, $8
	addl	$8, 0, $8
	cmpult	$8, $2, $22
	beq	$22, $939
	.loc	2 233
 #  233			{
	.loc	2 234
 #  234			fprintf(stderr,"Division would overflow\n");
	lda	$16, _iob+112
	lda	$17, $$21
	stl	$18, 32($sp)
	stq	$21, 104($sp)
	stq	$2, 96($sp)
	stl	$3, 24($sp)
	stq	$4, 112($sp)
	stq	$5, 64($sp)
	.livereg	0x0001E002,0x00000000
	jsr	$26, fprintf
	ldgp	$gp, 0($26)
	ldl	$18, 32($sp)
	ldq	$21, 104($sp)
	ldq	$2, 96($sp)
	ldl	$3, 24($sp)
	ldq	$4, 112($sp)
	ldq	$5, 64($sp)
	.loc	2 235
 #  235			abort();
	.livereg	0x00010002,0x00000000
	jsr	$26, abort
	ldgp	$gp, 0($26)
	ldl	$18, 32($sp)
	ldq	$21, 104($sp)
	ldq	$2, 96($sp)
	ldl	$3, 24($sp)
	ldq	$4, 112($sp)
	ldq	$5, 64($sp)
$939:
	ldil	$16, 64
	.loc	2 237
 #  236			}
 #  237		i=BN_BITS2-i;
	subl	$16, $18, $18
	.loc	2 238
 #  238		if (h >= d) h-=d;
	cmpult	$2, $4, $23
	bis	$4, $4, $24
	cmovne	$23, 0, $24
	subq	$2, $24, $2
	.loc	2 240
 #  239	
 #  240		if (i)
	beq	$18, $940
	.loc	2 241
 #  241			{
	.loc	2 242
 #  242			d<<=i;
	bis	$18, $18, $0
	sll	$4, $0, $4
	.loc	2 243
 #  243			h=(h<<i)|(l>>(BN_BITS2-i));
	sll	$2, $0, $25
	subl	$16, $18, $27
	srl	$21, $27, $6
	or	$25, $6, $2
	.loc	2 244
 #  244			l<<=i;
	sll	$21, $0, $21
$940:
	.loc	2 246
 #  245			}
 #  246		dh=(d&BN_MASK2h)>>BN_BITS4;
	and	$4, -4294967296, $20
	srl	$20, 32, $20
	.loc	2 247
 #  247		dl=(d&BN_MASK2l);
	.loc	2 248
 #  248		for (;;)
	and	$4, 4294967295, $1
	.align	3
$941:
	.loc	2 249
 #  249			{
	.loc	2 250
 #  250			if ((h>>BN_BITS4) == dh)
	srl	$2, 32, $7
	cmpeq	$20, $7, $8
	beq	$8, $942
	.loc	2 251
 #  251				q=BN_MASK2l;
	ldiq	$18, 4294967295
	br	$31, $943
$942:
	.loc	2 253
 #  252			else
 #  253				q=h/dh;
	divqu	$2, $20, $18
$943:
	.loc	2 255
 #  254	
 #  255			for (;;)
	mulq	$18, $20, $17
	negq	$20, $19
	subq	$2, $17, $0
	.align	3
$944:
	.loc	2 256
 #  256				{
	.loc	2 257
 #  257				t=(h-q*dh);
	bis	$0, $0, $16
	.loc	2 258
 #  258				if ((t&BN_MASK2h) ||
	and	$0, -4294967296, $22
	bne	$22, $945
	and	$21, -4294967296, $23
	srl	$23, 32, $24
	sll	$16, 29, $27
	s8addq	$27, $24, $25
	mulq	$1, $18, $6
	cmpult	$25, $6, $7
	bne	$7, $946
$945:
	.loc	2 262
 #  259					((dl*q) <= (
 #  260						(t<<BN_BITS4)+
 #  261						((l&BN_MASK2h)>>BN_BITS4))))
 #  262					break;
	mulq	$18, $1, $19
	br	$31, $947
$946:
	.loc	2 263
 #  263				q--;
	addq	$18, -1, $18
	addq	$17, $19, $17
	addq	$0, $20, $0
	br	$31, $944
$947:
	.loc	2 265
 #  264				}
 #  265			th=q*dh;
	.loc	2 266
 #  266			tl=q*dl;
	.loc	2 267
 #  267			t=(tl>>BN_BITS4);
	.loc	2 268
 #  268			tl=(tl<<BN_BITS4)&BN_MASK2h;
	.loc	2 269
 #  269			th+=t;
	.loc	2 271
 #  270	
 #  271			if (l < tl) th++;
	insll	$19, 4, $0
	srl	$19, 32, $8
	addq	$17, $8, $22
	cmpult	$21, $0, $23
	addq	$22, $23, $16
	.loc	2 272
 #  272			l-=tl;
	subq	$21, $0, $21
	.loc	2 273
 #  273			if (h < th)
	cmpult	$2, $16, $27
	beq	$27, $948
	.loc	2 274
 #  274				{
	.loc	2 275
 #  275				h+=d;
	addq	$2, $4, $2
	.loc	2 276
 #  276				q--;
	addq	$18, -1, $18
$948:
	.loc	2 278
 #  277				}
 #  278			h-=th;
	subq	$2, $16, $2
	.loc	2 280
 #  279	
 #  280			if (--count == 0) break;
	addl	$3, -1, $3
	beq	$3, $949
	.loc	2 280

	.loc	2 282
 #  281	
 #  282			ret=q<<BN_BITS4;
	sll	$18, 32, $5
	.loc	2 283
 #  283			h=((h<<BN_BITS4)|(l>>BN_BITS4))&BN_MASK2;
	sll	$2, 32, $24
	srl	$21, 32, $25
	or	$24, $25, $2
	.loc	2 284
 #  284			l=(l&BN_MASK2l)<<BN_BITS4;
	and	$21, 4294967295, $21
	sll	$21, 32, $21
	br	$31, $941
$949:
	.loc	2 286
 #  285			}
 #  286		ret|=q;
	or	$5, $18, $0
	.loc	2 287
 #  287		return(ret);
$50:
	.livereg	0xFC7F0002,0x3FC00000
	ldq	$26, 0($sp)
	lda	$sp, 144($sp)
	ret	$31, ($26), 1
	.end	bn_div64
