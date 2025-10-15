#include <stdio.h>
#include <sys/time.h>
#include <unistd.h>
#define SYSCALLS_COUNT 1000
int main()
{
    struct timeval start_time;
    gettimeofday(&start_time, NULL);
    FILE *test_file = fopen("./test.txt", "r");
    char buffer[1];
    for (size_t i = 0; i < SYSCALLS_COUNT; i++)
    {
        fread(&buffer, 0, 0, test_file);
    }
    struct timeval end_time;
    gettimeofday(&end_time, NULL);
    // tv_usec is the seconds between linux's epoch and current time
    // tv_sec is the microseconds between linux's epoch and current time
    long unsigned elapsed =
        (end_time.tv_sec * 1000000 + end_time.tv_usec) -
        (start_time.tv_sec * 1000000 + start_time.tv_usec);
    printf("%.5f Microseconds Elapsed", (float)elapsed / SYSCALLS_COUNT);
}