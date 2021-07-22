#include "stdarg.h"


struct BOOTINFO { /* 0x0ff0-0x0fff */
	char cyls; /* �u�[�g�Z�N�^�͂ǂ��܂Ńf�B�X�N��ǂ񂾂̂� */
	char leds; /* �u�[�g���̃L�[�{�[�h��LED�̏�� */
	char vmode; /* �r�f�I���[�h  ���r�b�g�J���[�� */
	char reserve;
	short scrnx, scrny; /* ��ʉ𑜓x */
	char *vram;
};
#define ADR_BOOTINFO	0x00000ff0



extern struct BOOTINFO *binfo;
extern unsigned char KanjiFont12b[7896+1][18];
extern unsigned char chara_gene[];		// 6x8 charactor, bit0 ����
extern int cur_x;
extern int cur_y;
extern int text_color;
extern int back_color;
extern int rev_flag;
