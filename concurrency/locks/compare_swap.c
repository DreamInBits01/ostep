#include <stdio.h>

/*
Locks are evaluated at 3 axes

*/
typedef struct lock_t
{
    int flag;
} lock_t;
int CompareAndSet(int *ptr, int expected, int new)
{
    int actual = *ptr;
    if (actual == expected)
    {
        *ptr = new;
    }
    return actual;
}
void init(lock_t *lock)
{
    lock->flag = 0;
};
void lock(lock_t *lock)
{
    /*
        If the value of the lock is as expect, then it's going to be set to the new value.
        For example, when the lock is not held, the expected value is 0, so the condition
        will evaluate to true and the new value will be one thus holding the lock.
        The returned vaue will be the previous value before the update
    */

    while (CompareAndSet(&lock->flag, 0, 1) == 1)
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