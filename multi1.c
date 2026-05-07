#include<stdio.h>

#define COUNT 10   //for RR

typedef struct Process {
    int pid; 
    int priority;
    int burst_time; 
    int queue_level; 
    int remaining_time;
    int arrival_time; 
    int waiting_time; 
    int Turnaround_time;
    int completed;
    struct Process* next;
} Process;

typedef struct Queue {
    Process* front;
    Process* rear;
    int count;
} Queue;

Process array[100]; 
Queue queues[4];
Queue SJFNP_Queue;
Process *arraySJPNONP[100];
int countSJFNP=0;
Process *arrayforTraversal[100];
int global_time = 0;

int isEmpty(Queue* q) {
    return (q->front == NULL);
}

void enqueue(Queue* q, Process* p) {
    p->next=NULL;
    if (q->rear == NULL) {
        q->front = q->rear = p;
    } else {
        q->rear->next = p;
        q->rear = p;
    }
    q->count++;
}

Process* dequeue(Queue* q) {
    if (isEmpty(q)) return NULL;
    Process* temp = q->front;
    q->front = q->front->next;
    if (q->front == NULL) {
        q->rear = NULL;
    }
    q->count--;
    temp->next = NULL;
    return temp;
}

// FIX 1: Corrected Traversal function - remove unused parameter
Process **Traversal(Queue *q, int * GT) {   
    countSJFNP=0;
    int count=0;
    Process * temp = q->front;
    
    while (temp != NULL) {   
        if(temp->arrival_time <= *GT && temp->completed != 1){
            arrayforTraversal[count] = temp;
            count++;
        }
        temp = temp->next;
    }
    countSJFNP = count;
    return arrayforTraversal;
}

int FindMinRemainingTime(Process **m){
    if(countSJFNP == 0) return 9999;
    int min_remaining_time = m[0]->remaining_time;
    int count = countSJFNP;
    for(int i=0; i<count; i++) { 
        if(min_remaining_time > m[i]->remaining_time) {
            min_remaining_time = m[i]->remaining_time;
        }
    }
    return min_remaining_time;
}

Process *Min_RemainingTime_Process(Process **m){
    int min_remaining_time = FindMinRemainingTime(m);
    int count = countSJFNP;
    for(int i=0; i<count; i++){
        if(min_remaining_time == m[i]->remaining_time){
            return m[i];
        }
    }
    return NULL;
}

// FIX 2: Improved SJF Non-Preemptive for Queue 2
int SJF_NON_Preemptive_Queue(Queue *q) {  // Removed unused p parameter
    int time_used = 0;
    int time_slice = 20;
    
    while(q->count != 0 && time_used < time_slice){
        // Get available processes
        while(countSJFNP == 0 && time_used < time_slice) {
            Traversal(q, &global_time);
            if(countSJFNP == 0 && time_used < time_slice) {
                // If no process available, find next arrival time
                int next_arrival = 9999;
                Process *temp = q->front;
                while(temp != NULL) {
                    if(temp->arrival_time > global_time && temp->arrival_time < next_arrival && temp->completed != 1) {
                        next_arrival = temp->arrival_time;
                    }
                    temp = temp->next;
                }
                
                if(next_arrival != 9999) {
                    int jump = next_arrival - global_time;
                    if(jump > time_slice - time_used) jump = time_slice - time_used;
                    global_time += jump;
                    time_used += jump;
                } else {
                    global_time++;
                    time_used++;
                }
            }
        }
        
        if(countSJFNP == 0) break;
        
        Process *current = Min_RemainingTime_Process(arrayforTraversal);
        if(current == NULL) break;
        
        int time_left = time_slice - time_used;
        int run_time = (current->remaining_time < time_left) ? current->remaining_time : time_left;
        
        current->remaining_time -= run_time;
        global_time += run_time;
        time_used += run_time;
        
        if(current->remaining_time == 0){
            current->completed = 1;
            current->Turnaround_time = global_time - current->arrival_time; 
            current->waiting_time = current->Turnaround_time - current->burst_time; 
            printf("[SJF NP Q2] Process %d completed at time %d\n", current->pid, global_time);
            
            // Remove completed process from linked list
            Process *scan = q->front, *sprev = NULL;
            while(scan != NULL && scan != current){ 
                sprev = scan; 
                scan = scan->next; 
            }
            if(scan != NULL){
                if(sprev == NULL) q->front = scan->next; 
                else sprev->next = scan->next;
                if(q->rear == scan) q->rear = sprev;
                scan->next = NULL;
            }
            q->count--;
            enqueue(&SJFNP_Queue, current);
        }
        countSJFNP = 0;
    }
    return time_used;
}

// FIX 3: Improved SJF Preemptive for Queue 1
int SJF_Preemptive_queue1(Queue *q) {  // Removed unused p parameter
    int time_used = 0;
    int time_slice = 20;
    
    while(time_used < time_slice && q->count > 0) {
        // Get available processes
        while (countSJFNP == 0 && time_used < time_slice) {
            Traversal(q, &global_time);
            if (countSJFNP == 0 && time_used < time_slice) {
                // Find next arrival time
                int next_arrival = 9999;
                Process *temp = q->front;
                while(temp != NULL) {
                    if(temp->arrival_time > global_time && temp->arrival_time < next_arrival && temp->completed != 1) {
                        next_arrival = temp->arrival_time;
                    }
                    temp = temp->next;
                }
                
                if(next_arrival != 9999) {
                    int jump = next_arrival - global_time;
                    if(jump > time_slice - time_used) jump = time_slice - time_used;
                    global_time += jump;
                    time_used += jump;
                } else {
                    global_time++;
                    time_used++;
                }
            }
        }

        if (countSJFNP == 0) break;
        
        Process* current = Min_RemainingTime_Process(arrayforTraversal);
        if(current == NULL) break;
        
        int run_time = 1;
        current->remaining_time -= run_time;
        global_time += run_time;
        time_used += run_time;
        
        if (current->remaining_time == 0) {
            current->completed = 1;
            current->Turnaround_time = global_time - current->arrival_time; 
            current->waiting_time = current->Turnaround_time - current->burst_time; 
            
            // Remove completed process from linked list
            Process *sc = q->front, *sp = NULL;
            while(sc != NULL && sc != current){ 
                sp = sc; 
                sc = sc->next; 
            }
            if(sc != NULL){
                if(sp == NULL) q->front = sc->next; 
                else sp->next = sc->next;
                if(q->rear == sc) q->rear = sp;
                sc->next = NULL;
            }
            q->count--;
            printf("[SJF Preemptive Q1] Process %d completed at time %d\n", current->pid, global_time);
        } else {
            printf("[SJF Preemptive Q1] Process %d preempted at time %d, remaining time: %d\n", 
                   current->pid, global_time, current->remaining_time);
        }
        countSJFNP = 0;
    }
    return time_used;
}

// FIX 4: Improved Round Robin for Queue 0
int RoundRobin_Queue() {
    int time_used = 0;
    int time_slice = 20;
    int rr_quantum = COUNT;

    while (time_used < time_slice && queues[0].count > 0) {
        int found = 0;
        Process* scan = queues[0].front;
        Process* current = NULL;
        Process* prev = NULL;
        
        // Find first available process
        while (scan != NULL) {
            if (scan->arrival_time <= global_time && scan->completed != 1) {
                found = 1;
                current = scan;
                break;
            }
            prev = scan;
            scan = scan->next;
        }

        if (!found) {
            // Find next arrival time
            int next_arrival = 9999;
            scan = queues[0].front;
            while (scan != NULL) {
                if (scan->arrival_time > global_time && scan->arrival_time < next_arrival && scan->completed != 1) {
                    next_arrival = scan->arrival_time;
                }
                scan = scan->next;
            }
            
            if(next_arrival != 9999) {
                int jump = next_arrival - global_time;
                if(jump > time_slice - time_used) jump = time_slice - time_used;
                global_time += jump;
                time_used += jump;
            } else {
                global_time++;
                time_used++;
            }
            continue;
        }

        int time_left_in_slice = time_slice - time_used;
        int run_time = rr_quantum;
        if (run_time > current->remaining_time) run_time = current->remaining_time;
        if (run_time > time_left_in_slice) run_time = time_left_in_slice;

        current->remaining_time -= run_time;
        global_time += run_time;
        time_used += run_time;

        if (current->remaining_time == 0) {
            current->completed = 1;
            current->Turnaround_time = global_time - current->arrival_time;
            current->waiting_time = current->Turnaround_time - current->burst_time;
            
            if (prev == NULL) {
                queues[0].front = current->next;
            } else {
                prev->next = current->next;
            }
            if (current->next == NULL) {
                queues[0].rear = prev;
            }
            current->next = NULL;
            queues[0].count--;
            printf("[RR Q0] Process %d completed at time %d\n", current->pid, global_time);
        } else {
            if (prev == NULL) {
                queues[0].front = current->next;
            } else {
                prev->next = current->next;
            }
            if (current->next == NULL) {
                queues[0].rear = prev;
            }
            current->next = NULL;
            queues[0].count--;
            enqueue(&queues[0], current);
            printf("[RR Q0] Process %d preempted at time %d, remaining: %d\n",
                   current->pid, global_time, current->remaining_time);
        }
    }
    return time_used;
}

// FIX 5: Improved FCFS for Queue 3
int FCFS_Queue() {
    int time_used = 0;
    int time_slice = 20;

    while (time_used < time_slice && !isEmpty(&queues[3])) {
        Process* current = queues[3].front;

        if (current->arrival_time > global_time) {
            int idle = current->arrival_time - global_time;
            if (idle > time_slice - time_used) idle = time_slice - time_used;
            global_time += idle;
            time_used += idle;
            continue;
        }

        int time_left = time_slice - time_used;
        int run_time = (current->remaining_time < time_left) ? current->remaining_time : time_left;

        current->remaining_time -= run_time;
        global_time += run_time;
        time_used += run_time;

        if (current->remaining_time == 0) {
            current->completed = 1;
            current->Turnaround_time = global_time - current->arrival_time;
            current->waiting_time = current->Turnaround_time - current->burst_time;
            dequeue(&queues[3]);
            printf("[FIFO Q3] Process %d completed at time %d\n", current->pid, global_time);
        } else {
            printf("[FIFO Q3] Process %d paused at time %d, remaining: %d\n",
                   current->pid, global_time, current->remaining_time);
            break;  // Break to give other queues a chance
        }
    }
    return time_used;
}

// FIX 6: Fixed is_queues_done function
int is_queues_done() {
    for (int i = 0; i < 100; i++) {
        if (array[i].burst_time > 0 && array[i].completed != 1) {
            return 0;
        }
    }
    return 1;
}

// FIX 7: Keep your original run_scheduling structure but add queue cycling
void run_scheduling() {
    int queue_order[] = {0, 1, 2, 3};  // Queue order for cycling
    int current_index = 0;
    
    while (!is_queues_done()) {
        int queue_processed = 0;
        
        // Try current queue in the cycle
        switch(queue_order[current_index]) {
            case 0:
                if (!isEmpty(&queues[0])) {
                    printf("\n[Time %d] Switching to Queue 0 (RR) for 20s\n", global_time);
                    RoundRobin_Queue();
                    queue_processed = 1;
                }
                break;
            case 1:
                if (!isEmpty(&queues[1])) {
                    printf("\n[Time %d] Switching to Queue 1 (SJF Preemptive) for 20s\n", global_time);
                    SJF_Preemptive_queue1(&queues[1]);
                    queue_processed = 1;
                }
                break;
            case 2:
                if (!isEmpty(&queues[2])) {
                    printf("\n[Time %d] Switching to Queue 2 (SJF Non-Preemptive) for 20s\n", global_time);
                    SJF_NON_Preemptive_Queue(&queues[2]);
                    queue_processed = 1;
                }
                break;
            case 3:
                if (!isEmpty(&queues[3])) {
                    printf("\n[Time %d] Switching to Queue 3 (FIFO) for 20s\n", global_time);
                    FCFS_Queue();
                    queue_processed = 1;
                }
                break;
        }
        
        // Move to next queue in cycle
        current_index = (current_index + 1) % 4;
        
        // If no queue had processes, advance time to next arrival
        if (!queue_processed) {
            int next_arrival = 9999;
            for (int i = 0; i < 100; i++) {
                if (array[i].burst_time > 0 && array[i].completed != 1) {
                    if (array[i].arrival_time < next_arrival && array[i].arrival_time > global_time) {
                        next_arrival = array[i].arrival_time;
                    }
                }
            }
            
            if (next_arrival != 9999) {
                printf("[IDLE] No processes ready, advancing time to %d\n", next_arrival);
                global_time = next_arrival;
            } else {
                global_time++;
            }
        }
    }
}

int main() {
    int n, i;
    printf("Enter number of processes: ");
    scanf("%d", &n);
    
    for(i=0; i<4; i++) {
        queues[i].front = queues[i].rear = NULL;
        queues[i].count = 0;
    }
    SJFNP_Queue.front = SJFNP_Queue.rear = NULL;
    SJFNP_Queue.count = 0;

    for (i = 0; i < n; i++) {
        printf("Enter PID:");
        scanf("%d", &array[i].pid);
        printf("Enter Priority (0-3):");
        scanf("%d", &array[i].priority);
        printf("Enter Burst Time:");
        scanf("%d", &array[i].burst_time);
        printf("Enter Arrival Time:");
        scanf("%d", &array[i].arrival_time);
        printf("\n");
        array[i].remaining_time = array[i].burst_time; 
        array[i].waiting_time = 0; 
        array[i].Turnaround_time = 0; 
        array[i].next = NULL; 
        array[i].completed = 0;
        array[i].queue_level = array[i].priority;

        int p = array[i].priority;
        enqueue(&queues[p], &array[i]);
    }
    
    printf("\n--- Starting Multilevel Queue Scheduling ---\n\n");
    run_scheduling();

    printf("\n--- Scheduling Results ---\n");
    printf("PID\tPriority\tBurst Time\tArrival Time\tWaiting Time\tTurnaround Time\n");
    for(i=0; i<n; i++){
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
            array[i].pid,
            array[i].priority,
            array[i].burst_time,
            array[i].arrival_time,
            array[i].waiting_time,
            array[i].Turnaround_time);
    }

    float total_wt=0, total_tat=0;
    for(i=0; i<n; i++){
        total_wt += array[i].waiting_time;
        total_tat += array[i].Turnaround_time;
    }
    printf("\nAverage Waiting Time     : %.2f\n", total_wt/n);
    printf("Average Turnaround Time  : %.2f\n", total_tat/n);

    return 0;
}