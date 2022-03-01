#include <stdio.h>

int main(int argc, char *argv[])
{
	int a = 0x7f000001;
	int ip[4] = {0,0,0,0};
	ip[0] = (a & 0xff000000) >> 24;
	ip[1] = (a & 0x00ff0000) >> 16;
	ip[2] = (a & 0x0000ff00) >> 8;
	ip[3] = (a & 0x000000ff);  
	printf("%x,%x,%x,%x",ip[0],ip[1],ip[2],ip[3]);
	return 0;
}

