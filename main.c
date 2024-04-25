#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#define MAX_NUM 25
#define getClock() ((double)clock() / CLOCKS_PER_SEC)

typedef struct operation {
    int job;
    int time;
    int machine;
} Operation;

typedef  struct job {
    int n;
    Operation operations[MAX_NUM];
} Job;

typedef struct solution
{
    int initialTime;
    int endTime;
} Solution;

typedef struct machine {
    int n;
    bool isFree;
    int endTime;
} Machine;

int main(int argc, char **argv) {
    FILE *inputFile;
    FILE *outputFIle;
    Job jobs[MAX_NUM];
    Machine  machines[MAX_NUM];

    int nrJobs;
    int nrMachines;
    int nrOfOperations;
    int cost = 0;

    inputFile = fopen(argv[1], "r");
    outputFIle = fopen("output.txt", "w");
    fscanf(inputFile, "%d%d", &nrJobs, &nrMachines);
    nrOfOperations = nrJobs * nrMachines;

    Solution solution[nrJobs][nrMachines];

    //    Display jobs, machines and operations

    printf("Jobs: %d\n", nrJobs);
    printf("Machines: %d\n", nrMachines);
    printf("OPerations: %d\n", nrOfOperations);

    //    Start Machines

    for (int m = 0; m < nrMachines; m++) {
        machines[m].isFree = true;
        machines[m].n = m;
    }

    // Read input file

    for (int i = 0; i < nrJobs; i++) {
        jobs[i].n = i;

        for (int j = 0; j < nrMachines; j++) {
            fscanf(inputFile, "%d%d", &jobs[i].operations[j].machine, &jobs[i].operations[j].time);
            jobs[i].operations[j].job = i;
        }
    }

    // Display input data

    for (int k = 0; k < nrJobs; k++) {

        printf("\n\nJob: %d  |  Operations: \n", jobs[k].n);

        for (int kk = 0; kk < nrMachines; k++) {
            printf("\n\nJob: %d  |  Operations: \n", jobs[k].n);
        }
    }

    double startTime = getClock();

    int maxEndTime = 0;

    // Scroll through columns

    for (int col = 0; col < nrMachines; col++) {
        for (int row = 0; row < nrJobs; row++) {

            // check if machine is free
            if (machines[jobs[row].operations[col].machine].isFree) {
                solution[row][col].initialTime = maxEndTime;
                // When the operation will end
                solution[row][col].endTime = solution[row][col].initialTime + jobs[row].operations[col].time;

                // put machine busy
                machines[jobs[row].operations[col].machine].isFree = false;
                // to know when the operation in machine will end
                machines[jobs[row].operations[col].machine].endTime = solution[row][col].endTime;
            } else {
                solution[row][col].initialTime = machines[jobs[row].operations[col].machine].endTime;
                solution[row][col].endTime = solution[row][col].initialTime + jobs[row].operations[col].time;

                // put machine busy
                machines[jobs[row].operations[col].machine].isFree = false;
                machines[jobs[row].operations[col].machine].endTime = solution[row][col].endTime;
            }
        }
        // Reset machines
        for (int m = 0; m < nrMachines; m++) {
            machines[m].isFree = true;
            machines[m].endTime = 0;
        }
        // find max time for columns interactions
        for (int mAux = 0; mAux < nrJobs; mAux++) {
            if (maxEndTime < solution[mAux][col].endTime) {
                maxEndTime = solution[mAux][col].endTime;
            }
        }
    }

    double endTime = getClock();

    //Print results
    printf("\n\n**************************");
    for (int sr = 0; sr < nrJobs; sr++) {
        printf("Job %d  |  ", sr);
        for (int sc = 0; sc < nrMachines; sc++) {
            printf("i: %d  e:%d | ", solution[sr][sc].initialTime, solution[sr][sc].endTime);
            fprintf(outputFIle, "%d", solution[sr][sc].initialTime );
        }
        printf("\n");
        fprintf(outputFIle, "\n");
    }
    printf("\n");
    printf("**************************");
    printf("\n\nTempo de execução (s)", endTime - startTime);

}