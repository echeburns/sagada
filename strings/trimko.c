#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

char *trim(char *str){

    char *end;
    
    while(*str == ' ' || *str == '\t' || *str == '\"') str++; //trim leading tabs and spaces 
    if(*str == 0)
        return str;
    
    end = str + strlen(str)-1; //trim trailing tabs, spaces and newline
    while(end > str && (*end == ' ' || *end == '\t' || *end == '\r' || *end == '\n' || *end == '\"' )) end--;
    
    *(end+1) = 0;

    return str;
}

#define MY_STRING "abc"
	
#define GET_STRING(a) #a 

int main(int argc, char **argv)
{
	char buf[64] = "\"Hello World!\"";
	char buf2[62] ="+CME ERROR: 111.";
	int rc;
	char buf3[64];
	char *buf4 = "MY_STRING";
	char rbuf[1024]="+CCID: 89630316041040035504\n";
	char tbuf[1024]={0};
	char *cp;
	int len=0;
	char *end = NULL;
    char error_code[6]={0};

	printf("%s\n",GET_STRING(buf4));
	
	printf("Before=[%s]\n",buf);	
	strcpy(buf,trim(buf));	
	printf("after=[%s]\n",buf);
	
	if(end != NULL && strstr(end, "hello") != NULL){
		printf("END is not NULL\n");
	}else{
		printf("END is NULL\n");
	}

	rc = strncmp(buf2, "+CME ERROR: 111.", 14);
	printf("strncmp=%d\n", rc);

	cp = buf2;
	while(!isdigit(*cp)) cp++; //trim leading tabs and spaces 
	
	end = cp + strlen(cp)-1; //trim trailing tabs, spaces and newline
    while(end > cp && (!isdigit(*end))) end--;
	*(end+1) = 0;

	printf("final=[%s] int=%d\n", cp, atoi(cp));

	cp = rbuf;
 	while(!isdigit(*cp)) cp++; //trim leading tabs and spaces 
	
	end = cp + strlen(cp)-1; //trim trailing tabs, spaces and newline
    while(end > cp && (!isdigit(*end))) end--;
	*(end+1) = 0;

	printf("final=[%s]\n", cp);

        
	return 0;
}

#if 0
#define MYSTRING "AAA"
char gbuf[16];

char *getName()
{
    char *buf = NULL;
    buf = gbuf;
    return buf;
}

int main(int argc, char **argv)
{
	char buf[16];
	char ch;
	char mac[18];
	char *p = &mac[0];
	char buffer1[16] = "";
	
	printf("[%s]\n", buffer1);
	strcpy(buffer1, "hello");
	printf("[%s]\n", buffer1);
	strcpy(&buffer1[0], "world");
	printf("[%s] [%c]\n", buffer1, buffer1[4]);
	
	strcpy(mac, "00-0B-4F-C6-D3-4C");
	
	/*sprintf(mac, "%c%c:%c%c:%c%c:%c%c:%c%c:%c%c", 
	mac[0], mac[1], mac[3],mac[4],mac[6],mac[7],mac[9],mac[10],mac[12],mac[13],mac[15],mac[16]);*/
	
	printf("mac=%s\n", mac);
	
	for(; *p; ++p) if(*p == '-') *p = ':';
	
	printf("mac=%s\n", mac);
	
	/*
	memset(buf,0,sizeof(buf));
	
	sprintf(buf,"%d", 'r');
	ch = atoi(buf);
	printf("buf=%s %d %c\n",buf, atoi(buf), ch);
	*/
    memset(gbuf,0x00,sizeof(gbuf));
    strcpy(gbuf,"AAA");
    
    printf("Compare[%s]gbuf[%s]\n", getName(),gbuf);
}
#endif
