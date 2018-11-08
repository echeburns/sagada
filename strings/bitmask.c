#include <stdio.h>

#define BIT_1 0x0001
#define BIT_2 0x0002
#define BIT_3 0x0004
#define BIT_4 0x0008

int func1(int i)
{
	switch(i)
	{
		case 1: return 10;
		case 2: return 20;
		case 3: return 30;
	}
	return 0;
}

main(int argc, char *argv[])
{
	int bitmask=0;
	char buf[4]={0};	
	char urcBuf[64]={0};
	char *cpBuf = urcBuf;
	int func2(func1(1));

	urcBuf[0] = 0;
	urcBuf[1] = 0;
	urcBuf[2] = 0x2b;
	urcBuf[3] = 0x43;
	urcBuf[4] = 0x49;
	urcBuf[5] = 0x45;	

	printf("0-%d\n", strlen(cpBuf+0));
	printf("1-%d\n", strlen(cpBuf+1));
	printf("2-%d\n", strlen(cpBuf+2));
	printf("3-%d\n", strlen(cpBuf+3));

	printf("answer: %d %d %d\n", 2%8, 16%8, 102%8 );
	bitmask = BIT_1 | BIT_2 | BIT_3 | BIT_4;
	
	printf("on ALL [%d]\n", bitmask);
	
	bitmask ^= BIT_3;
	
	printf("off BIT_3 [%d]\n", bitmask);
	
	buf[0] = 3; //number of input pending
	buf[1] = BIT_1; // Bit 0 - connect event; Bit 1 - disconnect; Bit 2 - RX ready
	buf[2] = 0; // number of output pending
	buf[3] = BIT_2; // framing error
	
	printf("[%d] [%d] [%d] [%d]\n", buf[0], buf[1], buf[2], buf[3]);
	
	printf("input pending [%d]\n", buf[0]);
	if(!buf[1])	printf("event cause - connect\n" );	
	if(buf[1] & BIT_1)	printf("event cause - disconnect\n" );	
	if(buf[1] & BIT_2)	printf("event cause - rx ready\n");	
	printf("output pending [%d]\n", buf[2]);

	bitmask = 8 << func2;
}