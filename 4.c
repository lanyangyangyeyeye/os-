#include <stdio.h>
#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>
#include <stdlib.h>

#define MSGKEY 75

struct msgform {
    long mtype;
    char mtrex[1024];
} msg;

int msgqid, i;

void CLIENT() {
    msgqid = msgget(MSGKEY, 0777 | IPC_CREAT);
    for (i = 10; i >= 1; i--) {
        msg.mtype = i;
        sprintf(msg.mtrex, "Message %d", i);
        printf("(client) sent\n");
        msgsnd(msgqid, &msg, sizeof(msg.mtrex), 0);
    }
    exit(0);
}

void SERVER() {
    msgqid = msgget(MSGKEY, 0777 | IPC_CREAT);
    do {
        msgrcv(msgqid, &msg, sizeof(msg.mtrex), 0, 0);
        printf("(server) received: %s\n", msg.mtrex);
    } while (msg.mtype != 1);
    msgctl(msgqid, IPC_RMID, 0);
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
            wait();
            wait();
        }
    }
    return 0;
}

