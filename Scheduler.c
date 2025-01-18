#include <stdint.h>
#include <stdio.h>
#include <limits.h>

#define MAX_TASKS UCHAR_MAX

// Task states
typedef enum 
{
    TASK_READY,
    TASK_RUNNING,
    TASK_SUSPENDED

} task_state_t;

// Task Control Block structure
typedef struct
{
    uint32_t *stack_pointer;    // Placeholder for stack pointer (not used here)
    uint8_t priority;           // Priority of the task (lower value = higher priority)
    task_state_t state;         // Current state of the task
    void (*task_func)(void);    // Function pointer to the task's main function

} tcb_t;

tcb_t task_list[MAX_TASKS];     // Array of task control blocks
uint8_t task_index = NULL;         // Index of the next task to be added   

// Initialize the scheduler
void init_scheduler() 
{
    for (int i = NULL; i < MAX_TASKS; i++) 
    {
        task_list[i].state = TASK_SUSPENDED;    // Set all tasks to suspended initially
    }

}

// Add a task to the scheduler
void add_task(void (*task_func)(void), uint8_t priority) 
{
    if (task_index < MAX_TASKS) 
    {
        task_list[task_index].task_func = task_func;
        task_list[task_index].priority = priority;
        task_list[task_index].state = TASK_READY;

        task_index++;

        for (int index_val = NULL; index_val < task_index - 1; index_val++)    // Sort tasks based on priority after each addition : Bubble sort
        {
            for (int sort_val = index_val + 1; sort_val < task_index; sort_val++) 
            {
                if (task_list[index_val].priority > task_list[sort_val].priority) 
                {
                    tcb_t temp = task_list[index_val];                      // Swap the tasks
                    task_list[index_val] = task_list[sort_val];
                    task_list[sort_val] = temp;
                }
            }
        }
    } 
    
    else 
    {
        printf("Task list is full!\n");
    }
}

void start_scheduler() 
{
    while (1) 
    {
        // Find the next ready task with the highest priority (lowest priority value)
        for (int task_val = NULL; task_val < task_index; task_val++) 
        {
            if (task_list[task_val].state == TASK_READY) 
            {
                task_list[task_val].state = TASK_RUNNING;
                task_list[task_val].task_func();
                task_list[task_val].state = TASK_READY; // Set task back to ready after execution
            }
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
    add_task(task3, 2);  // Task with priority 2
    add_task(task1, 3);  // Task with priority 3
    add_task(task2, 1);  // Task with priority 1

    printf("Starting scheduler...\n");
    start_scheduler();

    return 0;
}
