#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

typedef struct rw_lock
{
    sem_t mutex;
    sem_t write_mutex;
    int readers;
} rw_lock_t;

void rw_lock_init(rw_lock_t *rw_lock)
{
    rw_lock->readers = 0;
    sem_init(&rw_lock->mutex, 0, 1);
    sem_init(&rw_lock->write_mutex, 0, 1);
};
void rw_lock_aquire_write_mutex(rw_lock_t *rw_lock)
{
    sem_wait(&rw_lock->write_mutex);
};
void rw_lock_relase_write_mutex(rw_lock_t *rw_lock)
{
    sem_post(&rw_lock->write_mutex);
}
void rw_read_aquire_read_lock(rw_lock_t *rw_lock)
{
    sem_wait(&rw_lock->mutex);
    rw_lock->readers++;
    if (rw_lock->readers == 1) // First reader aquires lock
    {
        rw_lock_aquire_write_mutex(&rw_lock->write_mutex);
    }
    sem_post(&rw_lock->mutex);
}
void rw_read_relase_read_lock(rw_lock_t *rw_lock)
{
    sem_wait(&rw_lock->mutex);
    rw_lock->readers--;
    if (rw_lock->readers == 0) // Last reader releases lock
    {
        rw_lock_aquire_relase_mutex(&rw_lock->write_mutex);
    }
    sem_post(&rw_lock->mutex);
}