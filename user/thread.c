#include "user/thread.h"
#include "user/user.h"
#include "kernel/riscv.h"

#include <stddef.h>

void lock_init(struct lock_t* lock)
{
    lock->locked = 0;
}

void lock_acquire(struct lock_t* lock)
{
    while(__sync_lock_test_and_set(&lock->locked, 1) != 0)
      ;
}

void lock_release(struct lock_t* lock)
{
    __sync_lock_release(&lock->locked);
}

int thread_create(void *(start_routine)(void*), void *arg)
{
    char *mem = malloc(PGSIZE);
    if (mem == NULL)
    {
        printf("Could not allocate memory");
        return -1;
    }

    char *stack_ptr = mem + PGSIZE;

    int pid = clone(stack_ptr);

    if (pid < 0)
    {
        printf("Could not create child thread, PID%d", pid);
        free(mem);
        return -1;
    }
    else if (pid > 0)
    {
        return 0;
    }

    start_routine(arg);

    exit(0);

}
