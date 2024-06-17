#include "net.h"

struct Client_t { // 客戶端的資料結構
    int fd; // 串流代號
    pthread_t thread; // 線程 thread
};

struct Client_t clients[CMAX]; // 所有客戶端

void *serv(void *argu) {
    int ci = *(int*)argu;   //客戶的編號1~10
    int cfd = clients[ci].fd;   //客戶的唯一ID
    char buffer[SMAX];
    char input[SMAX];
    char ipath[SMAX], path[SMAX], cmd[SMAX], fullcmd[SMAX], pathFile[SMAX];
    getcwd(ipath, SMAX-1); // 取得初始路徑
    strcpy(path, ipath);   // path = ipath
    sprintf(pathFile, "%s/path.txt", ipath); // pathFile=<ipath>/path.txt
    while (1) { // 不斷等待使用者輸入命令並執行之
        memset(buffer, 0, sizeof(buffer));
        memset(input, 0, sizeof(input));
        printf("myshell:%s $ ", path); // 顯示提示訊息
        fgets(cmd, SMAX-1, stdin);    // 等待使用者輸入命令
        strtok(cmd, "\n");             // 切掉 \n
        if (strcmp(cmd, "exit")==0) return 0;
        sprintf(fullcmd, "cd %s;%s;pwd>%s", path, cmd, pathFile); // fullcmd = 切到 path; 使用者輸入的命令; 將路徑存入 pathFile 中。
        FILE *fp;
        // 使用popen執行命令並讀取輸出
        fp = popen(fullcmd, "r");
        while (fgets(buffer, sizeof(buffer), fp) != NULL) {
            // 輸出每一行輸出到控制台
            strtok(buffer,"\n");
            strcat(buffer," ");
            printf("%s", buffer);
            strcat(input, buffer);
        }
        for (int i=0; i<CMAX; i++) { // 廣播給其他人
            send(clients[i].fd, input, strlen(input)+1, 0); 
        }
        printf("\n");
        readText(pathFile, path, SMAX);// 讀 pathFile 檔取得路徑
        strtok(path, "\n");            // 切掉 \n
        pclose(fp);
    }
    close(cfd);
    clients[ci].fd = 0;
    return NULL;
}

// 將檔案讀入成為字串
int readText(char *file, char *text, int size) {
  FILE *f = fopen(file, "r");
  int n = fread(text, 1, size, f);
  fclose(f);
  return n;
}

//為每個客戶建立新的thread
void connectHandler(int sfd) {
    struct sockaddr_in raddr;
    socklen_t rAddrLen = sizeof(struct sockaddr);
    int cfd = accept(sfd, (struct sockaddr*) &raddr, &rAddrLen);
    for (int i=0; i<CMAX; i++) {
        if (clients[i].fd == 0) {
            memset(&clients[i], 0, sizeof(clients[i]));     //清空clients[i]
            clients[i].fd = cfd;
            pthread_create(&clients[i].thread, NULL, serv, &i);
            break;
        }
    }
}

int main(int argc, char *argv[]) {
    int port = atoi(argv[1]);
    printf("port=%d\n", port);
    int sfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in saddr, raddr;
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(port);
    char msg[SMAX];
    saddr.sin_addr.s_addr = INADDR_ANY;
    int rb = bind(sfd, (struct sockaddr*) &saddr, sizeof(struct sockaddr));
    assert(rb >= 0);
    int rl = listen(sfd, CMAX);
    assert(rl >= 0);
    memset(clients, 0, sizeof(clients));
    while (1) {
        connectHandler(sfd);
    }
    close(sfd);
    return 0;
}
