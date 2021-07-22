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

void sprintf_u(char **buf, unsigned int d)
{
	if(d / 10){
		sprintf_u(buf, d/10);
	}
	*(*buf)++ = '0' + d%10;
	**buf = 0;
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