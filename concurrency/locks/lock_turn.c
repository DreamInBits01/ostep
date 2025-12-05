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
    lock->turn++;
}
int main()
{
}

/*
lock_t mutex;
lock_init(lock); -> (ticket = 0, turn = 0)
Lock state
    -ticket = 0;
    -turn = 0;

-Thread 1
lock(&mutex) <- (ticket = 0, turn = 0)
    -FNA(&lock->ticket) <- ticket = 0
        -FSA(&mutex->ticket) -> 0, lock state is now (ticket = 1, turn = 0)
    -thread_turn is the returned value from FNA(&mutex->ticket) which is 0
    -While (thread_turn != mutex->turn) <- (thread_turn = 0, mutex->turn = 0), thread WILL NOT spin
            ; //spin

-Thread 2 is running while thread 1 is holding the lock
lock(&mutex) <- (ticket = 1, turn = 0)
    -FNA(&lock->ticket) <- ticket = 1
        -FSA(&mutex->ticket) -> 1, lock state is now (ticket = 2, turn = 0)
    -thread_turn is the returned value from FNA(&mutex->ticket) which is 1
    -While (thread_turn != mutex->turn) <- (thread_turn = 1, mutex->turn = 0), thread WILL spin
            ; //spin

-Thread 1 released the mutex
unlock(&mutex) <- (ticket = 2, turn = 0)
    -return (ticket = 2, turn = 1), it's now thread 2 turn

-Thread 2 is running after thread 1 called unlock
-Mutex is now (ticket = 2, turn 1)
-Thread was stuck in spinning block
-While (thread_turn != mutex->turn) <- (thread_turn = 1, mutex->turn = 1), thread WILL NOT spin
        ; //spin

unlock(&mutex) <- (ticket = 2, turn = 1)
    -return (ticket = 2, turn = 2)











*/