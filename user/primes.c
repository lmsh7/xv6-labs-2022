#include "kernel/types.h"
#include "user/user.h"

#define WRITE_END 1
#define READ_END 0

int main(int argc, char *argv[])
{
    int hd = 0;
    int queue[35];
    for (int i = 2; i <= 35; ++i)
    {
        queue[hd++] = i;
    }
    while (hd > 0)
    {
        int p[2];
        pipe(p);
        int pid = fork();
        if (pid < 0)
        {
            fprintf(2, "fork failed\n");
            exit(1);
        }
        else if (pid > 0)
        {
            close(p[READ_END]);
            for (int i = 0; i < hd; ++i)
            {
                write(p[WRITE_END], &queue[i], sizeof(int));
            }
            close(p[WRITE_END]);
            wait((int *)0);
            exit(0);
        }
        else
        {
            close(p[WRITE_END]);
            int tmp = -1;
            int prime = -1;
            hd = -1;
            while (read(p[READ_END], &tmp, sizeof(int)) > 0)
            {
                if (hd == -1)
                {
                    prime = tmp;
                    hd = 0;
                }
                else if (tmp % queue[hd] != 0)
                {
                    queue[hd++] = tmp;
                }
            }
            printf("prime %d\n", prime);
            close(p[READ_END]);
        }
    }
    exit(0);
}