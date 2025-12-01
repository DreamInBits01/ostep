#include <stdio.h>

/*
Locks are evaluated at 3 axes
1-Does it provide mutual exclusion?
2-Does it provide good fairness?
3-Does it provide good performance?

Spin locks provide mutual exclusion
*/
typedef struct lock_t
{
    int flag;
} lock_t;
int TestAndSet(int *ptr, int new)
{
    int old = *ptr;
    *ptr = new;
    return old;
}
void init(lock_t *lock)
{
    lock->flag = 0;
};
void lock(lock_t *lock)
{
    /*
     -When the value is one, that means the lock is held so the thread is going to spin
     -When the value is 0, that meansthe while loop will not work and the thread will continue to work
    */

    while (TestAndSet(&lock->flag, 1) == 1)
        ;
}
void unlock(lock_t *lock)
{
    lock->flag = 0;
}
int main()
{

    return 0;
}