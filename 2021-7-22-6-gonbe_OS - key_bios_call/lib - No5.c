#include "gonbe.h"
#include "function.h"



int ut_strlen(char *str)
{
	int i;
	
	i = 0;
	while(*str){
		i++;
		str++;
	}
	return i;
}

void sprintf_1x(char **buf, unsigned int d)
{
	d &= 0x0000000f;
	if(d < 10){
		*(*buf)++ = '0' + d;
		**buf = 0;
	}
	else{
		*(*buf)++ = 'A' + d - 10;
		**buf = 0;
	}
}

void sprintf_x(char **buf, unsigned int d)
{
	int i;
	
	for(i=0; i<8; i++){
		sprintf_1x(buf, d>>28);
		d <<= 4;
	}
}

void sprintf_u(char **buf, unsigned int d)
{
	if(d / 10){
		sprintf_u(buf, d/10);
	}
	*(*buf)++ = '0' + d%10;
	**buf = 0;
}

void printf_x(unsigned int d)
{
	char buffer[50];
	char *buf = buffer;
	
	sprintf_x(&buf, d);
	ut_puts(buffer);
}

void sprintf_d(char **buf, int d)
{
	if(d<0){
		**buf++ = '-';
		d = -d;
	}
	sprintf_u(buf, d);
}

void printf_u(unsigned int d)
{
	
	if(d / 10){
		printf_u(d / 10);
	}
	ut_putc('0' + d%10);
}


void printf_d(int d)
{
	if(d < 0){
		ut_putc('-');
		d = -d;
	}
	printf_u(d);
}

void ut_vsprintf(char *buf, char *fmt, va_list ap)
{
	char *p, *sval;
	int ival;
	
	for(p=fmt; *p; p++){
		if(*p != '%'){
			*buf++ = *p;
			continue;
		}
		switch(*++p){
			case 'c':
				ival = va_arg(ap, int);
				*buf++ = ival;
				*buf = 0;
				break;
			case 's':
				sval = va_arg(ap, char *);
				while(*sval){
					*buf++ = *sval++;
				}
				*buf = 0;
				break;
			case 'd':
				ival = va_arg(ap, int);
				sprintf_d(&buf, ival);
//				buf += ut_strlen(buf);
				break;
			case 'u':
				ival = va_arg(ap, unsigned int);
				sprintf_u(&buf, ival);
//				buf += ut_strlen(buf);
				break;
			default:
				*buf++ = '%';
				*buf++ = *p;
				break;
		}
	}
	*buf = 0;
}

void ut_printf(char *fmt, ...)
{
	va_list ap;
	char buf[1024];
	
	va_start(ap, fmt);
	ut_vsprintf(buf, fmt, ap);
	va_end(ap);
	ut_puts(buf);
}

char ut_getc_tbl[128]={
	0,		// 0x00
	0,
	0,
	0,
	0,		// 0x04
	0,
	0,
	0,
	0,		// 0x08
	0,
	0,
	0,
	0,		// 0x0c
	0,
	0,
	0,
	
	0,		// 0x10
	0,
	0,
	0,
	0,		// 0x14
	0,
	0,
	0,
	0,		// 0x18
	0,
	0,
	0,
	0,		// 0x1c
	0,
	0,
	0,
	
	0,		// 0x20
	0,
	0,
	0,
	0,		// 0x24
	0,
	0,
	0,
	0,		// 0x28
	0,
	0,
	0,
	0,		// 0x2c
	0,
	0,
	0,
	
	0,		// 0x30
	0,
	0,
	0,
	0,		// 0x34
	0,
	0,
	0,
	0,		// 0x38
	0,
	0,
	0,
	0,		// 0x3c
	0,
	0,
	0,
	
	0,		// 0x40
	0,
	0,
	0,
	0,		// 0x44
	0,
	0,
	0,
	0,		// 0x48
	0,
	0,
	0,
	0,		// 0x4c
	0,
	0,
	0,
	
	0,		// 0x50
	0,
	0,
	0,
	0,		// 0x54
	0,
	0,
	0,
	0,		// 0x58
	0,
	0,
	0,
	0,		// 0x5c
	0,
	0,
	0,
	
	0,		// 0x60
	0,
	0,
	0,
	0,		// 0x64
	0,
	0,
	0,
	0,		// 0x68
	0,
	0,
	0,
	0,		// 0x6c
	0,
	0,
	0,
	
	0,		// 0x70
	0,
	0,
	0,
	0,		// 0x74
	0,
	0,
	0,
	0,		// 0x78
	0,
	0,
	0,
	0,		// 0x7c
	0,
	0,
	0,
	
};

int ut_getc()
{
	int code;
	
	while((key_input() & 0x80)==0)	// キーから離されるのを待つ
		;
	while((code=key_input()) & 0x80)	// キーが押されるまで待つ
		;
	code &= 0x7f;
	
	return ut_getc_tbl[code] & 0xff;
}
