#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(demo, LOG_LEVEL_DBG);

#define STACK_SIZE 1024

#define PRIO_A      5
#define PRIO_B      5
#define INCREMENTS  1000000

static struct k_sem done_sem;
static struct k_mutex counter_mutex;
static volatile uint32_t counter=0;

void worker_fn(void *p1, void *p2, void *p3)
{   
    const char *name = k_thread_name_get(k_current_get());

    for (int i=0; i < INCREMENTS; i++) {
        k_mutex_lock(&counter_mutex, K_FOREVER);
        counter++;
        k_mutex_unlock(&counter_mutex);
    }

    LOG_INF("[%s] finished its work", name);
    k_sem_give(&done_sem);
}


K_THREAD_DEFINE(thread_a, STACK_SIZE, worker_fn,
                NULL, NULL, NULL, PRIO_A, 0, 0);
K_THREAD_DEFINE(thread_b, STACK_SIZE, worker_fn,
                NULL, NULL, NULL, PRIO_B, 0, 0);

int main(void)
{
    k_mutex_init(&counter_mutex);
    k_sem_init(&done_sem, 0, 2);

    LOG_INF("Expected final counter value: %u", INCREMENTS*2);

    k_sem_take(&done_sem, K_FOREVER);
    k_sem_take(&done_sem, K_FOREVER);

    LOG_INF("Actual final counter value: %u", counter);

    if (counter == INCREMENTS*2) {
        LOG_WRN("No race this run!");
    }
    else {
        LOG_ERR("Race condition detected! Lost %u updates!", 
            INCREMENTS*2 - counter);
    }

    return 0;
}

