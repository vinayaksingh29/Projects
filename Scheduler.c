#include <stdio.h>
#include <stdint.h>
#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define MAX_TASKS 5

typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_SUSPENDED
} task_state_t;

typedef struct {
    uint8_t priority;
    task_state_t state;
    void (*task_func)(void);
} tcb_t;

tcb_t task_list[MAX_TASKS];
uint8_t task_index = 0;
timer_t timer_id;
int current_task = 0;

// Task functions
void task1() { {printf("Task 1 running (Priority: %d)\n", task_list[current_task].priority); } }
void task2() { {printf("Task 2 running (Priority: %d)\n", task_list[current_task].priority); } }
void task3() { {printf("Task 3 running (Priority: %d)\n", task_list[current_task].priority); } }

// Comparator function for sorting tasks by priority
int compare_tasks(const void *a, const void *b) {
    return ((tcb_t *)a)->priority - ((tcb_t *)b)->priority; // Lower priority value = higher priority
}

// Scheduler function called by the timer
void scheduler(int sig) {
    if (sig == SIGALRM) {
        if (task_index == 0) return; // No tasks to run

        // Run the current task
        task_list[current_task].state = TASK_RUNNING;
        task_list[current_task].task_func();
        task_list[current_task].state = TASK_READY;

        // Move to the next task in sorted order
        current_task = (current_task + 1) % task_index;
    }
}

// Function to start POSIX timer
void start_timer(int time_ms) {
    struct sigevent sev;
    struct itimerspec its;

    sev.sigev_notify = SIGEV_SIGNAL;
    sev.sigev_signo = SIGALRM;
    timer_create(CLOCK_REALTIME, &sev, &timer_id);

    its.it_value.tv_sec = time_ms / 1000;
    its.it_value.tv_nsec = (time_ms % 1000) * 1000000;
    its.it_interval.tv_sec = its.it_value.tv_sec;
    its.it_interval.tv_nsec = its.it_value.tv_nsec;

    timer_settime(timer_id, 0, &its, NULL);
}

// Adding task to scheduler (sorted insertion)
void add_task(void (*task_func)(void), uint8_t priority) {
    if (task_index < MAX_TASKS) {
        task_list[task_index].task_func = task_func;
        task_list[task_index].priority = priority;
        task_list[task_index].state = TASK_READY;
        task_index++;

        // Sort tasks based on priority
        qsort(task_list, task_index, sizeof(tcb_t), compare_tasks);
    } else {
        printf("Task list full!\n");
    }
}

// Main function
int main() {
    signal(SIGALRM, scheduler);

    add_task(task3, 2);
    add_task(task2, 1); // Highest priority (lower number)
    add_task(task1, 3);

    printf("Starting scheduler...\n");

    start_timer(100); // Timer interval in ms

    while (1) { sleep(1); } // Keep main thread alive

    return 0;
}
