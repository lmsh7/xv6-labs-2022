#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p_pipe[2], c_pipe[2];
    if (pipe(p_pipe) == -1 || pipe(c_pipe) == -1)
    {
        fprintf(2, "pipe failed\n");
        exit(1);
    }
    int pid = fork();
    if (pid == -1)
    {
        fprintf(2, "fork failed\n");
        exit(1);
    }

    if (pid == 0)
    {
        char buffer[256];
        read(p_pipe[0], buffer, sizeof(buffer));
        char pong_msg[] = "%d: received ping\n";
        fprintf(1, pong_msg);
        fprintf(c_pipe[1], pong_msg, getpid());
    }
    else
    {
        char ping_msg[] = "ping";
        write(p_pipe[1], ping_msg, sizeof(ping_msg));
        char buffer[256];
        read(c_pipe[0], buffer, sizeof(buffer));
        fprintf(1, "%d: received pong\n", getpid());
    }
    exit(0);
}