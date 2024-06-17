#include <unistd.h>
#include <stdlib.h>
#include <stdio.h> 

void print_george() {    // 每隔一秒鐘印出一次 George 的函數
  while (1) {    
    printf("George\n");    
    sleep(1);    
  }        
}    

void print_mary() {     // 每隔2秒鐘印出一次 Mary 的函數
  while (1) {    
    printf("Mary\n");    
    sleep(2);    
  }       
}    

int main() {     // 主程式開始
    if (fork()==0)
    {
        print_mary();
    }
    else{
        if (fork()==0)
        {
            print_george();
        }
        else{
            while (1) {     // 主程式每隔一秒鐘
                printf("----------------\n");    // 就印出分隔行
                sleep(1);     // 停止一秒鐘
            } 
        }
    }
    return 0;    
}