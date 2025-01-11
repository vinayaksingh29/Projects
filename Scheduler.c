#include <stdio.h>
#include <stdint.h>

// Task states
typedef enum {
    TASK_READY,
    TASK_RUNNING,
    TASK_SUSPENDED
} task_state_t;

// Task Control Block structure
typedef struct {
    uint32_t *stack_pointer;  // Points to the task's stack (not used in this simple example)
    uint8_t priority;         // Task priority (lower is higher priority)
    task_state_t state;       // Current state of the task
    void (*task_func)(void);  // Pointer to the task's main function
} tcb_t;

#define MAX_TASKS 5

tcb_t task_list[MAX_TASKS];  // Array to store tasks
uint8_t current_task = 0;    // Tracks the current task index

// Initialize the scheduler
void init_scheduler() {
    for (int i = 0; i < MAX_TASKS; i++) {
        task_list[i].state = TASK_SUSPENDED;  // Initially, all tasks are inactive
    }
}

// Add a task to the scheduler
void add_task(void (*task_func)(void), uint8_t priority) {
    static uint8_t task_index = 0;
    if (task_index < MAX_TASKS) {
        task_list[task_index].task_func = task_func;
        task_list[task_index].priority = priority;
        task_list[task_index].state = TASK_READY;
        task_index++;
    } else {
        printf("Task list is full!\n");
    }
}

// Start the scheduler
void start_scheduler() {
    while (1) {
        int highest_priority_task = -1;
        uint8_t highest_priority = 255; // Start with the lowest possible priority value

        // Find the task with the highest priority that is ready
        for (int i = 0; i < MAX_TASKS; i++) {
            if (task_list[i].state == TASK_READY && task_list[i].priority < highest_priority) {
                highest_priority = task_list[i].priority;
                highest_priority_task = i;
            }
        }

        // If a ready task is found, execute it
        if (highest_priority_task != -1) {
            current_task = highest_priority_task;
            task_list[current_task].state = TASK_RUNNING;
            task_list[current_task].task_func(); // Run the task
            task_list[current_task].state = TASK_READY; // Set back to ready after execution
        }
    }
}

// Example tasks
void task1() {
    printf("Task 1 is running\n");
}

void task2() {
    printf("Task 2 is running\n");
}

void task3() {
    printf("Task 3 is running\n");
}

int main() {
    init_scheduler();
    add_task(task1, 3); // Task 1 has priority 3
    add_task(task2, 1); // Task 2 has priority 1 (highest priority)
    add_task(task3, 2); // Task 3 has priority 2

    printf("Starting scheduler...\n");
    start_scheduler();

    return 0;
}
