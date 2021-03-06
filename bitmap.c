#include "bitmap.h"

int checkb(int fd, int iorb){
        if (iorb == 0) { //for inode bitmap
                lseek(fd, 1024, SEEK_SET);
               // unsigned char* buf;
	       // buf = (unsigned char*) malloc(1024);
                unsigned char* buf = malloc(1024);
		read(fd, buf, 1024);
                for (int byte = 0; byte < 1024; byte++) {
                        unsigned char comp = 128;
                        int count = 0;
                        if (buf[byte] != 255) {
                                while ((buf[byte] & comp)!=0) {
                                    comp = comp >> 1;
                                    count++;
                                }
                                free(buf);
                                if((byte*8+count)<0){
                                        printf("bitmap Error");
                                }

                                return  byte * 8 + count;
                        }
                }

        }
        else if (iorb == 1){ //for data bitmap
                lseek(fd,2048, SEEK_SET);
                //unsigned char* buf;
		//buf= (unsigned char*)malloc(1024*128);
                unsigned char* buf = malloc(1024 * 128);
		read(fd, buf, 1024*128);
                for(int byte = 0; byte<1024*128;byte++){
                        unsigned char comp = 128;
                        int count = 0;
                        if (buf[byte] != 255) {
                                while ((buf[byte] & comp)!=0) {
                                    comp = comp >> 1;
                                    count++;
                                }
                                free(buf);
                                if((byte*8+count)<0){
                                        printf("bitmap Error");
                                }
                                return  byte * 8 + count;
                        }
                }
        }
        else{
                printf("Parameter should be 0 or 1!");
		return -1;
        }
	return 0;
}
void chanb(int fd, int num, int iorb) { // this funcion change bitmap of num 0->1, or 1->0
        unsigned char* buf = malloc(1);
        if (iorb == 0) {
                lseek(fd, 1024 + (num/8), SEEK_SET);
        }
        else {
                lseek(fd, 2048 + (num/8), SEEK_SET);
        }
        read(fd, buf, 1);
        lseek(fd, -1, SEEK_CUR);
        unsigned char com = num % 8;
        com = 128 >> com;
        unsigned char new = buf[0] ^ com;
        buf[0] = new;
        write(fd, buf, 1);
        free(buf);
}

int onoffcheck(int fd, int num, int iorb) { // this function check on/off on bitmap of num. if return value is 0, bitmap is off and 1 means that bitmap is on.
        unsigned char* buf = malloc(1);
        if (iorb == 0) {
                lseek(fd, 1024 + (num/8), SEEK_SET);
        }
        else {
                lseek(fd, 2048 + (num/8), SEEK_SET);
	}
	read(fd, buf, 1);
	unsigned char new = buf[0];
	free(buf);
	return new;
}
