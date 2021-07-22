// srec_to_img gonbe_rear.srec gonbe_rear.img		# gonbe_near.img は 0x8000番地からの生データ

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <io.h>
#include <memory.h>


char ram_buffer[2000000];

/********************************************************************************/
/*		f_mgr_prg																*/
/********************************************************************************/
unsigned int f_mgr_prg_s_addr;
int f_mgr_prg_buf[512/sizeof(int)];
int f_mgr_prg_buf_addr;
int f_mgr_prg_buf_addr_valid;

int f_mgr_prg_sub(int rom_addr, int ram_addr)
{
	memcpy(&ram_buffer[rom_addr], (char*)ram_addr, 512);
	memset((char*)ram_addr, 0, 512);
	return 0;
}

int f_mgr_prg_write(unsigned int addr, char data)
{
	unsigned int start, offset;
	
//    if(addr<0x9d000000 || addr>=0x9d040000)
//        return 0;                           // success アドレス外
	start = addr/512*512;
	offset = addr - start;
	if(f_mgr_prg_buf_addr_valid==0){		// 初回のみ
		f_mgr_prg_buf_addr_valid = 1;
		memset(f_mgr_prg_buf, 0xff, sizeof(f_mgr_prg_buf));
		f_mgr_prg_buf_addr = start;
		((char*)f_mgr_prg_buf)[offset] = data;
	}
	else{
		if(f_mgr_prg_buf_addr != start){	// 新しいバッファアドレスに移るから、旧バッファをメモリに書き込む
			if(f_mgr_prg_sub(f_mgr_prg_buf_addr, (int)f_mgr_prg_buf)){	// error
				printf("*** flush memory write error\n");
				return 1;					// error
			}
			memset(f_mgr_prg_buf, 0x00, sizeof(f_mgr_prg_buf));
			f_mgr_prg_buf_addr = start;
		}
		((char*)f_mgr_prg_buf)[offset] = data;
	}
	return 0;								// success
}

int hex2dec(char h)
{
	if(h>='0' && h<='9')
		return h-'0';
	else if(h>='A' && h<='F')
		return h-'A'+10;
	else if(h>='a' && h<='f')
		return h-'a'+10;
	else{
		printf("*** hex2dec illegal charactor %02x\n", h);
		return 0;
	}
}

int s_chksum(unsigned char *buf, int byte)
{
	int i, sum;
	
	sum = 0;
	for(i=1; i<=byte+1; i++){
		sum += hex2dec(buf[i*2])*16 + hex2dec(buf[i*2+1]);
	}
	if((sum & 0xff) != 0xff){
		printf("*** error chksum, %s %02x\n", buf, sum);
		return 0;							// error
	}
	return 1;								// success
}

//	'S' Type ByteCount Address Data Checksum

int s_file_loader(char *filename)
{
	FILE *fd_file_in;
	unsigned char buf[256];
	int i, byte, err, data;
	unsigned int addr;
		
	err = 1;										// preset success
	fd_file_in = fopen(filename, "r");
	if(fd_file_in == 0){
		printf("*** %s open error\n", filename);
		return 0;					// error
	}
	for(;;){
		if(fgets((char*)buf, 256, fd_file_in) == 0)
			break;
//		fprintf(1000, "%s", buf);
		if(buf[0] != 'S'){
			printf("*** top charactor not \'S\'\n");
			printf("%s\n", buf);
			continue;
		}
		switch(buf[1]){
		case '0':						// 'S0' Header
			byte = hex2dec(buf[2])*16 + hex2dec(buf[3]);
			if(s_chksum(buf, byte)==0){	// if error
				err = 0;				// error
				break;
			}
			break;
		case '1':						// 'S1' Data  16-bit Address
			byte = hex2dec(buf[2])*16 + hex2dec(buf[3]);
			if(s_chksum(buf, byte)==0){	// if error
				err = 0;				// error
				break;
			}
			addr = 0;
			for(i=0; i<4; i++){
				addr = addr*16 + hex2dec(buf[i+4]);
			}
			for(i=4; i<4+byte-3; i++){
				data = hex2dec(buf[i*2])*16 + hex2dec(buf[i*2+1]);
//                *(unsigned char *)addr++ = data;								//####################
				if(f_mgr_prg_write(addr++, data)){
					err = 0;			// error
					break;
				}
			}
			break;
		case '2':						// 'S2' Data 24-bit Address
			byte = hex2dec(buf[2])*16 + hex2dec(buf[3]);
			if(s_chksum(buf, byte)==0){	// if error
				err = 0;				// error
				break;
			}
			addr = 0;
			for(i=0; i<6; i++){
				addr = addr*16 + hex2dec(buf[i+4]);
			}
			for(i=5; i<5+byte-4; i++){
				data = hex2dec(buf[i*2])*16 + hex2dec(buf[i*2+1]);
//				*(unsigned char *)addr++ = data;
				if(f_mgr_prg_write(addr++, data)){
					err = 0;			// error
					break;
				}
			}
			break;
		case '3':						// 'S3' Data 32-bit Address
			byte = hex2dec(buf[2])*16 + hex2dec(buf[3]);
			if(s_chksum(buf, byte)==0){	// if error
				err = 0;				// error
				break;
			}
			addr = 0;
			for(i=0; i<8; i++){
				addr = addr*16 + hex2dec(buf[i+4]);
			}
			for(i=6; i<6+byte-5; i++){
				data = hex2dec(buf[i*2])*16 + hex2dec(buf[i*2+1]);
//				*(unsigned char *)addr++ = data;
				if(f_mgr_prg_write(addr++, data)){
					err = 0;			// error
					break;
				}
			}
			break;
		case '7':						// 'S7'  Start Address  32-bit Address
			byte = hex2dec(buf[2])*16 + hex2dec(buf[3]);
			if(s_chksum(buf, byte)==0){	// if error
				err = 0;				// error
				break;
			}
			addr = 0;
			for(i=0; i<8; i++){
				addr = addr*16 + hex2dec(buf[i+4]);
			}
			f_mgr_prg_s_addr = addr;
			printf("=== start addr=%08x\n", addr);
			break;
		case '9':						// 'S9'  Start Address  16-bit Address
			byte = hex2dec(buf[2])*16 + hex2dec(buf[3]);
			if(s_chksum(buf, byte)==0){	// if error
				err = 0;				// error
				break;
			}
			addr = 0;
			for(i=0; i<4; i++){
				addr = addr*16 + hex2dec(buf[i+4]);
			}
			f_mgr_prg_s_addr = addr;
			printf("=== start addr=%04x\n", addr);
            break;
		}
	}
	fclose(fd_file_in);
	if(f_mgr_prg_sub(f_mgr_prg_buf_addr, (int)f_mgr_prg_buf)){
		printf("*** flush memory write error\n");
		return 0;						// error
	}

	return err;							// 1:success 0:error
}

int main(int argc, char *argv[])
{
	int fd;

	if(argc != 3){
		printf("***  $ srec_to_img.exe srec_file img_file\n");
		return 0;
	}
	f_mgr_prg_buf_addr_valid = 0;
	if(s_file_loader(argv[1])){
		printf("=== srec read success\n");
		printf("start addr = 0x%08X\n", f_mgr_prg_s_addr);
	}
	else{
		printf("*** program write error\n");
	}
	
	fd = _open(argv[2], O_WRONLY|O_BINARY|O_CREAT|O_TRUNC, 0777);
	if(fd < 0){
		printf("*** output file open error\n");
		return 0;
	}
	_write(fd, &ram_buffer[0x8000], 1440*1024-1024);
	
	_close(fd);
	return 0;
}
