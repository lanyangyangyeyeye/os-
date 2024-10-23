#include <stdio.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>

#define SHMKEY 75

int shmid, i;
int *addr;

void CLIENT() {
    shmid = shmget(SHMKEY, 1024, 0777 | IPC_CREAT);
    addr = shmat(shmid, 0, 0);
    for (i = 9; i >= 0; i--) {
        while (*addr != -1);
        printf("(client) sent: %d\n", i);
        *addr = i; // 将 i 写入共享内存
    }
    exit(0);
}

void SERVER() {
    shmid = shmget(SHMKEY, 1024, 0777 | IPC_CREAT);
    addr = shmat(shmid, 0, 0);
    do {
        *addr = -1; // 设置共享内存标志
        while (*addr == -1);
        printf("(server) received: %d\n", *addr);
    } while (*addr != 0);
    shmctl(shmid, IPC_RMID, 0);
    exit(0);
}

int main() {
    while ((i = fork()) == -1);
    if (i == 0) {
        SERVER();
    } else {
        while ((i = fork()) == -1);
        if (i == 0) {
            CLIENT();
        } else {
            wait(NULL);
            wait(NULL);
        }
    }
    return 0;
}

