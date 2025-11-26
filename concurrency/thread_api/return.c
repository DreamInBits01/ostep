#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
/*
pthread_join
    - Blocks the execution of the main thread until the joined thread is complete
    - Retrieves the Return Value when the thread exists via pthread_exit or return
    - Resource cleanup
    - Can only join once
    * It's similar to the wait system call used in parents processes to wait for children
        the parent waits for the child process to complete and then continue executing
*/
typedef struct
{
    int a;
    int b;
} args_t;
typedef struct
{
    int a_sucess;
    int b_sucess;
} return_t;
void *routine(void *data)
{
    args_t *args = (args_t *)data;
    printf("Thread running...\n");
    printf("A:%d, B:%d\n", args->a, args->b);
    return_t *ret = malloc(sizeof(return_t));
    ret->a_sucess = 1;
    ret->b_sucess = 1;
    return (void *)ret;
}
int main()
{
    pthread_t thread1;
    args_t args = {10, 20};
    return_t *ret;
    printf("Main is running...\n");
    pthread_create(&thread1, NULL, routine, &args);

    // Main thread blocks here until 'thread' finishes
    pthread_join(thread1, (void **)&ret);
    // Only executes after thread completes
    printf("A success:%d, B success:%d\n", ret->a_sucess, ret->b_sucess);
    free(ret);
}