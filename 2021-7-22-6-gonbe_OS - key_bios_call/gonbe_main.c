#include "gonbe.h"
#include "function.h"


//struct BOOTINFO { /* 0x0ff0-0x0fff */
//	char cyls; /* �u�[�g�Z�N�^�͂ǂ��܂Ńf�B�X�N��ǂ񂾂̂� */
//	char leds; /* �u�[�g���̃L�[�{�[�h��LED�̏�� */
//	char vmode; /* �r�f�I���[�h  ���r�b�g�J���[�� */
//	char reserve;
//	short scrnx, scrny; /* ��ʉ𑜓x */
//	char *vram;
//};
//#define ADR_BOOTINFO	0x00000ff0



struct BOOTINFO *binfo;
char stack_buf[100000];
extern char hankaku[4096];
int hoge_gonbe_main=1;

void init();

int gonbe_main(void)
{
	int i;
	
	init();
	enable_interrupt();
	
	text_color = 15;
	ut_puts("=== key_input test\n");

	for(;;){
		i = ut_getc();
		ut_putc(i);
	}
	
	while(1){
	}
}

void init()
{
	binfo = (struct BOOTINFO *)ADR_BOOTINFO;	// ���̍s�͕K���c���Ă���
	dispinit();
}
