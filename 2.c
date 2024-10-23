#include <unistd.h>
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

void waiting();
void stop(int signum);  // 更正函数声明

int wait_mark;

int main()  // 显式指定返回类型
{
    int p1, p2;

    while ((p1 = fork()) == -1);  // 创建第一个子进程
    if (p1 > 0)
    {
        while ((p2 = fork()) == -1);  // 创建第二个子进程
        if (p2 > 0)
        {
            printf("parent\n");
            wait_mark = 1;
            signal(SIGINT, stop);
            waiting();  // 等待信号
            kill(p1, SIGUSR1);  // 使用标准信号
            kill(p2, SIGUSR2);
            wait(0);  // 等待子进程结束
            wait(0);
            printf("parent process is killed!\n");
            exit(0);
        }
        else
        {
            printf("p2\n");
            signal(SIGINT, SIG_IGN);  // 忽略 SIGINT
            wait_mark = 1;
            signal(SIGUSR2, stop);  // 使用标准信号
            waiting();
            printf("child process 2 is killed by parent!\n");
            exit(0);
        }
    }
    else
    {
        printf("p1\n");
        signal(SIGINT, SIG_IGN);  // 忽略 SIGINT
        wait_mark = 1;
        signal(SIGUSR1, stop);  // 使用标准信号
        waiting();
        printf("child process 1 is killed by parent!\n");
        exit(0);
    }

    return 0;  // 返回值
}

void waiting()
{
    while (wait_mark != 0);
}

void stop(int signum)  // 增加信号参数
{
    wait_mark = 0;  // 结束等待
}

