#include "gonbe.h"
#include "function.h"


int cur_x;				// 文字単位のカーソル位置
int cur_y;				// 文字単位のカーソル位置
int last_cur_x;				// 6 pixcel を 1 文字幅とする
int last_cur_y;				// 16 pixcel を 1 文字高とする
char cur_data[8];			// カーソル置き換えデータ
int text_color;
int back_color;
int rev_flag;				// 0:text_color が文字色、back_color がバック色   1:text_color と back_color の役割反転

//extern uint32_t __attribute__((section(".mySection1"))) video[320][20];	// video RAM



/********************************************************************************/
/*	dispinit								*/
/********************************************************************************/
void dispinit()
{
	cur_x = 0;
	cur_y = 0;
	text_color = 15;
	back_color = 0;
	last_cur_x = binfo->scrnx / 6;
	last_cur_y = binfo->scrny / 16;
	rev_flag = 0;
}
/********************************************************************************/
/*		sjis_parse																*/
/*		文字列から１文字（1/2 バイトコード）取り出し							*/
/*		return: キャラゼネコード												*/
/********************************************************************************/
int sjis_parse(const char *str, int *byte)
{
	int c, ku, ten;
	
	c = *str & 0xff;
	if(c>=0x81 && c<=0x9f){
		ku = (c - 0x81)*2 + 1;				// 1～62
	}
	else if(c>=0xe0 && c<=0xef){			// 63～94
		ku = (c - 0xe0)*2 + 63;
	}
	else{
		*byte = 1;							// 1 バイトコード
		return c;
	}
	
	c = str[1] & 0xff;
	if(c>=0x40 && c<=0x7e){					// 1～63
		ten = c - 0x40 + 1;
	}
	else if(c>=0x80 && c<=0x9e){			// 64～94
		ten = c - 0x80 + 64;
	}
	else if(c>=0x9f && c<=0xfc){			// 1～94
		ku++;
		ten = c - 0x9f + 1;
	}
	else{									// 1 バイトコード  エラー
		*byte = 1;
		return c;
	}
	
    *byte = 2;
	return (ku-1)*94 + ten /* - 1 */;
}
/********************************************************************************/
/*		sjis_strlen																*/
/********************************************************************************/
int sjis_strlen(const char *str)
{
	int i, byte;
	
	for(i=0; ; ){
		if(sjis_parse(str, &byte)==0)
			return i;
		i++;
		str += byte;
	}
}
/********************************************************************************/
/*	dispchar								*/
/*	半角文字表示								*/
/*	rev_flag: 1なら text_color と back_color を入れ替える			*/
/********************************************************************************/
void dispchar(int x, int y, int c)
{
	int x1, y1, c1;
	
	c &= 0xff;
	if(c<0x20)
		return;
	for(x1=0; x1<6; x1++){
		c1 = chara_gene[(c-0x20)*6+x1];
		for(y1=0; y1<8; y1++){
			if(c1 & (1<<y1)){	// CG のビットがセットされている
				if(rev_flag==0){
					pset(x+x1, y+y1, text_color);
				}
				else{
					pset(x+x1, y+y1, back_color);
				}
			}
			else{			// CG のビットがクリアされている
				if(rev_flag==0){
					pset(x+x1, y+y1, back_color);
				}
				else{
					pset(x+x1, y+y1, text_color);
				}
			}
		}
	}
}

//	c: 句点コード 0～
//	12x16 の文字サイズ
void disp_sjis(int x, int y, int c)
{
	int i, j, bit, x1, y1, c1;
	
	for(i=0; i<12*16; i++){
		x1 = i%12;
		y1 = i/12;
		j = (i-24)/8;
		bit = 0x80 >> ((i-24)%8);
		c1 = KanjiFont12b[c][j];
		if((c1 & bit) && (i>=24 && i<12*14)){	// CG のビットが立っていて範囲内
			pset(x+x1, y+y1, rev_flag==0 ? text_color : back_color);
		}
		else{
			pset(x+x1, y+y1, rev_flag==0 ? back_color : text_color);
		}
	}
}

#if 0
void dispchar_xn(int x, int y, int c, int n)
{
	int x1, y1, c1;
	
    c &= 0xff;
	if(c<0x20 || c>0xff)
		return;
	for(x1=0; x1<6*n; x1++){
		c1 = chara_gene[(c-0x20)*6+x1/n];
		for(y1=0; y1<8*n; y1++){
			if(rev_flag==0){
				Pset(x+x1, y+y1, c1 & (1<<y1/n));
			}
			else{
				Pset(x+x1, y+y1, (c1 & (1<<y1/n))==0);
			}
		}
	}
}
#endif

//	半角専用
void dispstr(int x, int y, const char * str)
{
	while(*str){
		dispchar(x, y, *str++);
		x += 6;
	}
}

/********************************************************************************/

//	char cur_x
//	char cur_y
//	char cur_data: カーソル置き換えデータ

//	カーソル位置に元のグラフィックデータを書き込む
void clear_cur()
{
	int i, x, y;
	
	x = cur_x * 6;
	y = cur_y * 16 + 14;
	for(i=0; i<6; i++){
		pset(x++, y, cur_data[i]);
	}
}

//	カーソル位置のグラフィックデータを読み取る
void read_cur()
{
	int i, x, y;
	
	x = cur_x * 6;
	y = cur_y * 16 + 14;
	for(i=0; i<6; i++){
		cur_data[i] = pget(x++, y);
	}
}

void disp_cur()
{
	int i, x, y;
	
	x = cur_x * 6;
	y = cur_y * 16 + 14;
	for(i=0; i<6; i++){
		pset(x++, y, 15);
	}
}

void disp_U()
{
	clear_cur();
	--cur_y;
	if(cur_y < 0){
		cur_y = 0;
	}
	read_cur();
	disp_cur();
}

void disp_D()
{
	int x, y;
	
	clear_cur();
	++ cur_y;
	if(cur_y >= last_cur_y){
		cur_y = last_cur_y - 1;
		for(y=16; y<binfo->scrny; y++){
			for(x=0; x<binfo->scrnx; x++){
				pset(x,y-16, pget(x,y));
			}
		}
	}
	read_cur();
	disp_cur();
}

void disp_R()
{
	clear_cur();
	++cur_x;
	if(cur_x >= last_cur_x){
		cur_x = 0;
		read_cur();
		disp_D();
		return;
	}
	read_cur();
	disp_cur();
}

void disp_L()
{
	clear_cur();
	--cur_x;
	if(cur_x < 0){
		cur_x = last_cur_x - 1;
		read_cur();
		disp_U();
		return;
	}
	read_cur();
	disp_cur();
}

//	半角のみ
void ut_putc(unsigned char c)
{
	int x, y;
	
	x = cur_x * 6;
	y = cur_y * 16 + 4;
	if(c<0x20){
		if(c=='\n'){
			clear_cur();
			cur_x = 0;
			read_cur();
			disp_D();
			return;
		}
		else if(c=='\t'){					// 4文字毎のタブ
			ut_putc(' ');
			while(cur_x & 0x03){
				ut_putc(' ');
			}
		}
		else if(c=='\b'){
			disp_L();
		}
	}
	else{
		dispchar(x, y, c);
		disp_R();
	}
}

//	全角　半角混在
void ut_puts(const char *str)
{
	int byte, c, x, y;
	
	for(;;){
		c = sjis_parse(str, &byte);
		if(c==0)
			return;
		if(byte==1){
			ut_putc(c);
//			x = cur_x * 6;
//			y = cur_y * 16+4;
//			char_disp(x, y, c, 0);
//			disp_R();
			str++;
		}
		else{
			if(cur_x <= last_cur_x - 2){
				x = cur_x * 6;
				y = cur_y * 16;
				disp_sjis(x, y, c);
			}
			else{
				ut_putc('\n');
				x = cur_x * 6;
				y = cur_y * 16;
				disp_sjis(x, y, c);
			}
			disp_R();
			disp_R();
			str += 2;
		}
	}
	
//	while(*str) disp_1char(*str++);
}

#if 0
void ut_printf(const char *fmt, ...)
{
	char buf[256];
	va_list ap;

	va_start(ap, fmt);
	vsprintf(buf, fmt, ap);
	va_end(ap);
	
    disp_str(buf);
}
#endif

void cursor_set(int x, int y)
{
	clear_cur();
	if(x<0)
		x = 0;
	if(x>=last_cur_x)
		x = last_cur_x - 1;
	if(y<0)
		y = 0;
	if(y>=last_cur_y)
		y = last_cur_y - 1;
	cur_x = x;
	cur_y = y;
	read_cur();
	disp_cur();
}
/********************************************************************************/
/*	pset									*/
/********************************************************************************/
void pset(int x, int y, int color)
{
	if(x<0 || x>=binfo->scrnx || y<0 || y>=binfo->scrny)
		return;
	binfo->vram[y * binfo->scrnx + x] = color;
}

int pget(int x, int y)
{
	if(x<0 || x>=binfo->scrnx || y<0 || y>=binfo->scrny)
		return 0;
	return binfo->vram[y * binfo->scrnx + x];
}
