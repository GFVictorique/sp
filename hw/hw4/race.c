#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define LOOPS 1000000000
int money = 5000;

void *ATM1()
{
  for (int i=0; i<LOOPS; i++) {
    money+=500; 
  }
  return NULL;
}

void *ATM2()
{
  for (int i=0; i<LOOPS; i++) {
    money-=500;
  }
  return NULL;
}


int main() 
{
  pthread_t thread1, thread2;

  pthread_create(&thread1, NULL, ATM1, NULL);
  pthread_create(&thread2, NULL, ATM2, NULL);

  pthread_join( thread1, NULL);
  pthread_join( thread2, NULL);
  if (money!=5000)
  {
    printf("err\n");
  }
  else printf("money=%d\n",money);
  
}
