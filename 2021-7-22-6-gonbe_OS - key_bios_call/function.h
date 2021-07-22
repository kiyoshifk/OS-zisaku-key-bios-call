//	asm_lib.nasm
void enable_interrupt();
int  key_input();

//	lib.c
void printf_u(unsigned int d);
void printf_d(int d);
void printf_x(unsigned int d);
int ut_getc();
void ut_printf(char *fmt, ...);
int ut_strlen(char *str);
void ut_vsprintf(char *buf, char *fmt, va_list ap);

//	sjis.c
void dispinit();
void clear_cur();
void cursor_set(int x, int y);
void disp_cur();
void disp_D();
void disp_L();
void disp_R();
void disp_sjis(int x, int y, int c);
void disp_U();
void dispchar(int x, int y, int c);
void dispinit();
void dispstr(int x, int y, const char * str);
void pset(int x, int y, int color);
int pget(int x, int y);
void read_cur();
int sjis_parse(const char *str, int *byte);
int sjis_strlen(const char *str);
void ut_putc(unsigned char c);
void ut_puts(const char *str);
