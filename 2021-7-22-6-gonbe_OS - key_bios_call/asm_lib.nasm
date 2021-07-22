; naskfunc
; TAB=4

[BITS 32]						; 32ビットモード用の機械語を作らせる

	GLOBAL	_enable_interrupt
	GLOBAL	_key_input, key_input_return

[SECTION .text]

_enable_interrupt:	; void enable_interrupt();
	sti
	ret


_key_input:	; int key_input(void);
;	mov	eax,cr0
;	and	eax,0xfffffffe	;bit0(PEビット)を0にする（リアルモード移行のため）
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
		IN		 AL,0x60 		; から読み(受信バッファが悪さをしないように)
		JNZ		waitkbdout		; ANDの結果が0でなければwaitkbdoutへ
		RET

key_input_return:
	ret