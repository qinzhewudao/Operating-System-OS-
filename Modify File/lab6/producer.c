#define   __LIBRARY__  
#include <unistd.h>  
#include <stdlib.h>  
#include <stdio.h>  
#include <sys/types.h>  
#include <fcntl.h>  
  
  
#define EINVAL      22  
#define ENOMEM      12  
#define N           600  
#define KEYNUM      10  
#define PAGE        4096  

_syscall2(int,sem_open, const char*, name, unsigned int , value)  
_syscall1(int, sem_wait, sem_t  *, sem)  
_syscall1(int, sem_post, sem_t  *, sem)  
_syscall1(int, sem_unlink, const char  *, name)  
_syscall2(int, shmget, key_t, key, size_t, size)  
_syscall2(void *, shmat, int, trueaddress, const void*, shmaddr)  
  
  
int main()  
{  
        int trueaddress, i,* offsetaddress;  
        sem_t   *empty, *full, *mutex;  
        empty = (sem_t *)sem_open("empty",10);  
        full  = (sem_t *)sem_open("full", 0);  
        mutex = (sem_t *)sem_open("mutex", 1);  
        trueaddress = shmget((key_t)KEYNUM,PAGE);  
        if(trueaddress==-EINVAL) {  
                printf("larger than size of one page!");  
                goto exits;  
        }else if(trueaddress==-ENOMEM)  {  
                printf("no free page!");   
                goto exits;  
        }  
        else offsetaddress = (int*)shmat(trueaddress,NULL);  
  
  
        for( i = 0 ; i < N; i++) {  
                sem_wait(empty);  
                sem_wait(mutex);  
                offsetaddress[i%10] = i;  
                sem_post(mutex);  
                sem_post(full);  
       }  
exits:  
        fflush(stdout);  
        sem_unlink("empty");  
        sem_unlink("full");  
        sem_unlink("mutex");  
        return 0;  
}  