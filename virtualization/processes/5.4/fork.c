#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <wait.h>
int main(int argc, char **argv)
{
    pid_t child_pid = fork();

    if (child_pid < 0)
    {
        perror("Error mf, the child is aborted!\n");
        exit(1);
    }
    if (child_pid == 0)
    {
        close(STDOUT_FILENO);
        open("./wc-output.output", O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
        execvp(*(argv + 1), argv + 1);
    }
    else
    {
        wait(NULL);
    }
    return 0;
}