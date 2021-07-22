;------------------------------------------------------------------------
;	org	0x7e00

[SECTION .text-key_mouse]

;[BITS 32]
;	mov	esi,0xa0000
;	mov	al,3
;L3:
;	mov	[esi],al
;	inc	esi
;	cmp	esi,0xa8000
;	jnz	L3
;	jmp	dword 2*8:key_input_return-0x00280000
	
;L4:	hlt
;	jmp	L4


[BITS 16]

	EXTERN	key_input_return

;	mov	ax,0xa000
;	mov	es,ax
;	mov	bx,0
;L3:
;	mov	byte [es:bx],3
;	inc	bx
;	cmp	bx,0x8000
;	jnz	L3
;	jmp	dword 2*8:key_input_return-0x00280000

;L4:	hlt
;	jmp	L4

;;;----------------------------------------------------------------

;;;	LGDT	[GDTR0]			; �b��GDT��ݒ�
	mov	eax,cr0
	and	eax,0xfffffffe	;bit0(PE�r�b�g)��0�ɂ���i���A�����[�h�ڍs�̂��߁j
	mov	cr0,eax

	jmp	key_input_flash
key_input_flash:
	mov	ax,0
	mov	ds,ax
	mov	es,ax
;	mov	fs,ax
;	mov	gs,ax
	mov	ss,ax

	CALL	waitkbdout
	MOV	AL,0xd1
	OUT	0x64,AL
	CALL	waitkbdout
;;	MOV	AL,0xdf			; enable A20
	mov	al,0xdd			; disable A20
	OUT	0x60,AL
	CALL	waitkbdout

	jmp	0:key_input_real
key_input_real:
	;************************* 16bit real mode code
	mov	ah,0x21
	int	0x16
	;************************* 16bit real mode code
	push	ax		;al:ascii, ah:scan code

	CALL	waitkbdout
	MOV		AL,0xd1
	OUT		0x64,AL
	CALL	waitkbdout
	MOV		AL,0xdf			; enable A20
;;;	mov		al,0xdd			; disable A20
	OUT		0x60,AL
	CALL	waitkbdout

;;;	LGDT	[GDTR0]			; �b��GDT��ݒ�
	MOV	EAX,CR0
	AND	EAX,0x7fffffff	; bit31��0�ɂ���i�y�[�W���O�֎~�̂��߁j
	OR	EAX,0x00000001	; bit0��1�ɂ���i�v���e�N�g���[�h�ڍs�̂��߁j
	MOV	CR0,EAX
	JMP	key_flash
key_flash:
	MOV	AX,1*8			;  �ǂݏ����\�Z�O�����g32bit
	MOV	DS,AX
	MOV	ES,AX
	MOV	FS,AX
	MOV	GS,AX
	MOV	SS,AX
	pop	ax
	
	jmp	dword 2*8:key_input_return-0x00280000

waitkbdout:
		IN		 AL,0x64
		AND		 AL,0x02
		IN		 AL,0x60 		; ����ǂ�(��M�o�b�t�@�����������Ȃ��悤��)
		JNZ		waitkbdout		; AND�̌��ʂ�0�łȂ����waitkbdout��
		RET





	times	0x200-($-$$)	db 0
