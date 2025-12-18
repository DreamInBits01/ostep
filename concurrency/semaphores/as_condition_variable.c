#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
sem_t condition;
/*
Semaphores are atomic counters that can be used to achieve mutual exclusion.
If the counter reached 0, the code after it won't execute


Tracing (child runs first)
-initial counter value is 0
-Child runs
    -It runs

-Parent runs (while child is running)
    -Decrease the counter, it becomes -1
    -The parent waits

-Child
    -Increments the counter it became 1 now
    -Exits

-Parent runs (child has completed)
    -Decreases the counter, it becomes 0
    -The thread continue running


Tracing (Parent runs first)
-initial counter value is 0
-Parent
    -Decrements the counter
    -It becomes zero so the parent waits
-Child runs
    -Child runs
    -Finishes, and increments the counter, it becomes 0
    -Exists

-Parent runs

*/
void *child(void *args)
{
    printf("Child run\n");
    printf("Child completed\n");
    sem_post(&condition); // incerments thee semaphore counter
}
int main()
{
    sem_init(&condition, 0, 0);
    printf("Parent is running...\n");
    pthread_t child_id;
    pthread_create(&child_id, NULL, child, NULL);
    sem_wait(&condition);
    printf("Parent ended...\n");
};