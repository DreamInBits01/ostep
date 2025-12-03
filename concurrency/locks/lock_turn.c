#include <stdio.h>

typedef struct lock_t
{
    int ticket;
    int turn;
} lock_t;
// Fetch and add
int FNA(int *ptr)
{
    int old = *ptr;
    *ptr = old + 1;
    return old;
}
void init_lock(lock_t *lock)
{
    lock->ticket = 0;
    lock->turn = 0;
}
void lock(lock_t *lock)
{
    int my_turn = FNA(&lock->ticket);
    while (my_turn != lock->turn)
        ;
}
void unlock(lock_t *lock)
{
    lock->turn = FNA(&lock->turn);
}
int main()
{
}