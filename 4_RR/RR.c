#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int remaining_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} Process;

void calculate_times(Process processes[], int n, int quantum) {
    int current_time = 0;
    int completed = 0;

    while (completed != n) {
        for (int i = 0; i < n; i++) {
            if (processes[i].arrival_time <= current_time && processes[i].remaining_time > 0) {
                if (processes[i].remaining_time <= quantum) {
                    current_time += processes[i].remaining_time;
                    processes[i].remaining_time = 0;
                    processes[i].completion_time = current_time;
                    processes[i].turnaround_time = processes[i].completion_time - processes[i].arrival_time;
                    processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
                    completed++;
                } else {
                    current_time += quantum;
                    processes[i].remaining_time -= quantum;
                }
            }
        }
    }
}

void print_processes(Process processes[], int n) {
    printf("PID\tTime de llegada\tTimepo de Rafaga\tTimepo Competicion\tTurnaround Time\tTimepo Espera\n");
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t\t%d\t\t%d\t\t%d\t\t%d\n",
               processes[i].pid,
               processes[i].arrival_time,
               processes[i].burst_time,
               processes[i].completion_time,
               processes[i].turnaround_time,
               processes[i].waiting_time);
    }
}

int main() {
    int n, quantum;

    printf("Introduzca el número de procesos: ");
    scanf("%d", &n);
    
    Process *processes = (Process *)malloc(n * sizeof(Process));

    for (int i = 0; i < n; i++) {
        processes[i].pid = i + 1;
        printf("Introduzca la hora de llegada del proceso %d: ", i + 1);
        scanf("%d", &processes[i].arrival_time);
        printf("Introduzca el tiempo de ráfaga para el proceso %d: ", i + 1);
        scanf("%d", &processes[i].burst_time);
        processes[i].remaining_time = processes[i].burst_time;
    }

    printf("Introduzca el cuanto de tiempo: ");
    scanf("%d", &quantum);

    calculate_times(processes, n, quantum);
    print_processes(processes, n);

    free(processes);

    return 0;
}
