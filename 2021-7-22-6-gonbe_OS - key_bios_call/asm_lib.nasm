; naskfunc
; TAB=4

[BITS 32]						; 32�r�b�g���[�h�p�̋@�B�����点��

	GLOBAL	_enable_interrupt
	GLOBAL	_key_input, key_input_return

[SECTION .text]

_enable_interrupt:	; void enable_interrupt();
	sti
	ret


_key_input:	; int key_input(void);
;	mov	eax,cr0
;	and	eax,0xfffffffe	;bit0(PE�r�b�g)��0�ɂ���i���A�����[�h�ڍs�̂��߁j
;	mov	cr0,eax
;
;	jmp	key_input_flash
;key_input_flash:
;	mov	ax,0
;	mov	ds,ax
;	mov	es,ax
;;	mov	fs,ax
;;	mov	gs,ax
;	mov	ss,ax
;
;	CALL	waitkbdout
;	MOV	AL,0xd1
;	OUT	0x64,AL
;	CALL	waitkbdout
;;;	MOV	AL,0xdf			; enable A20
;	mov	al,0xdd			; disable A20
;	OUT	0x60,AL
;	CALL	waitkbdout

	jmp	dword 3*8:0x7e00
	
	
waitkbdout:
		IN		 AL,0x64
		AND		 AL,0x02
		IN		 AL,0x60 		; ����ǂ�(��M�o�b�t�@�����������Ȃ��悤��)
		JNZ		waitkbdout		; AND�̌��ʂ�0�łȂ����waitkbdout��
		RET

key_input_return:
	ret