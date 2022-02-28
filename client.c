#include <stdio.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
int main(int argc, char *argv[])
{
	int clientfd;
	int re;
	struct sockaddr_in client_addr;
	clientfd = socket(AF_INET, SOCK_STREAM, 0);
	bzero(&client_addr,sizeof(client_addr));
	client_addr.sin_port = htons(1234);
	client_addr.sin_family = AF_INET;
	client_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	if(connect(clientfd,(struct sockaddr*)&client_addr, sizeof(client_addr)) < 0 ){
		perror("connect error");
		exit(-1);
	}
	while(1){
		
	}

	return 0;
}
