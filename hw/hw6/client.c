#include "net.h"

int main(int argc, char *argv[]) {
	char *ip = argv[1];
	int port = atoi(argv[2]);
	int sfd = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in saddr, raddr;
	memset(&saddr, 0, sizeof(saddr));
	memset(&raddr, 0, sizeof(raddr));
	saddr.sin_family = AF_INET;
	saddr.sin_port = htons(port);
	saddr.sin_addr.s_addr = inet_addr(ip);
	int rc = connect(sfd, (struct sockaddr*) &saddr, sizeof(struct sockaddr));
	assert(rc >= 0);
	printf("connect to server success!\n");
	
	while (1)
	{
		char msg[SMAX];
		int n = recv(sfd, msg, SMAX - 1, 0);
		if (n <=0) break;
		if (n!=1)
		{
			printf("Execution result:\n");
    		printf("%s\n\n", msg);
		}
	}
	close(sfd);
	return 0;
}