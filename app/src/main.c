#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

LOG_MODULE_REGISTER(demo, LOG_LEVEL_DBG);

#define STACK_SIZE 1024

#define PRIO_LOW    7
#define PRIO_MED    5
#define PRIO_HIGH   3
#define PRIO_COOP   (-1)


void low_fn(void *p1, void *p2, void *p3)
{
    LOG_INF("[LOW] starting");
    
    while (1) {
        LOG_INF("[LOW] loop");
        k_msleep(300);
    }
}

void med_fn(void *p1, void *p2, void *p3)
{
    LOG_INF("[MED] starting");

    while (1) {
        LOG_INF("[MED] loop");
        k_msleep(200);
    }
}


void high_fn(void *p1, void *p2, void *p3)
{
    LOG_INF("[HIGH] starting");

    while (1) {
        LOG_INF("[HIGH] loop");
        k_msleep(100);
    }
}

void coop_fn(void *p1, void *p2, void *p3)
{
    LOG_INF("[COOP] starting");

    for (uint16_t i=1; i<=5; i++) {
        LOG_INF("[COOP] iteration %d", i);
        k_busy_wait(20000);
    }

    LOG_INF("[COOP] yielding...");
    k_yield();

    LOG_INF("[COOP] work done!");
}

K_THREAD_DEFINE(t_high_fn, STACK_SIZE, high_fn,
                NULL, NULL, NULL, PRIO_HIGH, 0, 0);
K_THREAD_DEFINE(t_med_fn, STACK_SIZE, med_fn,
                NULL, NULL, NULL, PRIO_MED, 0, 0);
K_THREAD_DEFINE(t_low_fn, STACK_SIZE, low_fn,
                NULL, NULL, NULL, PRIO_LOW, 0, 0);
K_THREAD_DEFINE(t_coop_fn, STACK_SIZE, coop_fn,
                NULL, NULL, NULL, PRIO_COOP, 0, 0);


int main(void)
{
    return 0;
}

