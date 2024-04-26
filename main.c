#include <stdio.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>
#define MAX_NUM 25
#define getClock() ((double)clock() / CLOCKS_PER_SEC)

typedef struct operation
{
    int job;
    int time;
    int machine;
} Operation;

typedef struct job
{
    int jobsNumber;
    Operation operations[MAX_NUM];
} Job;

typedef struct solution
{
    int initialTime;
    int endTime;
} Solution;

typedef struct machine
{
    int machineNumber;
    bool isFree;
    int endTime;
} Machine;

int main(int argc, char **argv)
{
    FILE *inputFile;
    FILE *outputFile;
    Job jobs[MAX_NUM];
    Machine machines[MAX_NUM];

    int nrJobs;
    int nrMachines;
    int nrOfOperations;

    inputFile = fopen(argv[1], "r");
    outputFile = fopen("output.txt", "w");
    fscanf(inputFile, "%d%d", &nrJobs, &nrMachines);
    nrOfOperations = nrJobs * nrMachines;

    Solution solution[nrJobs][nrMachines];

    // Display jobs, machines and operations
    printf("Jobs: %d\n", nrJobs);
    printf("Machines: %d\n", nrMachines);
    printf("Operations: %d\n", nrOfOperations);

    // Start machines
    for (int m = 0; m < nrMachines; m++)
    {
        machines[m].isFree = true;
        machines[m].machineNumber = m;
    }

    // Read input file
    for (int i = 0; i < nrJobs; i++)
    {
        jobs[i].jobsNumber = i;

        for (int j = 0; j < nrMachines; j++)
        {
            fscanf(inputFile, "%d%d", &jobs[i].operations[j].machine, &jobs[i].operations[j].time);
            jobs[i].operations[j].job = i;
        }
    }

    // Display input data
    for (int k = 0; k < nrJobs; k++)
    {
        printf("\n\nJob: %d   | Operations: \n", jobs[k].jobsNumber);

        for (int kk = 0; kk < nrMachines; kk++)
        {
            printf("J%d M%d T%d | ", jobs[k].operations[kk].job, jobs[k].operations[kk].machine, jobs[k].operations[kk].time);
        }
    }

    double startTime = getClock();

    int maxEndTime = 0;
    // scroll through columns
    for (int col = 0; col < nrMachines; col++)
    {
        // scroll through lines
        for (int row = 0; row < nrJobs; row++)
        {
            // Check if machine is free
            if (machines[jobs[row].operations[col].machine].isFree)
            {
                solution[row][col].initialTime = maxEndTime;
                // End Time: When operation will finish
                solution[row][col].endTime = solution[row][col].initialTime + jobs[row].operations[col].time;

                // put machine busy
                machines[jobs[row].operations[col].machine].isFree = false;
                // Aux -> Para saber quando a operação na máquina em questão vai terminar
                machines[jobs[row].operations[col].machine].endTime = solution[row][col].endTime;
            }
            else
            {
                solution[row][col].initialTime = machines[jobs[row].operations[col].machine].endTime;
                solution[row][col].endTime = solution[row][col].initialTime + jobs[row].operations[col].time;

                // Put machine busy
                machines[jobs[row].operations[col].machine].isFree = false;
                machines[jobs[row].operations[col].machine].endTime = solution[row][col].endTime;
            }
        }

        // Reset machines
        for (int m = 0; m < nrMachines; m++)
        {
            machines[m].isFree = true;
            machines[m].endTime = 0;
        }

        //  Find maximum time for columns interaction
        for (int mAux = 0; mAux < nrJobs; mAux++)
            if (maxEndTime < solution[mAux][col].endTime)
                maxEndTime = solution[mAux][col].endTime;
    }

    double endTime = getClock();

    // Print Results
    printf("\n\n*********************");
    printf("\n\nSolution:\n");
    for (int sr = 0; sr < nrJobs; sr++)
    {
        printf("Job %d | ", sr);
        for (int sc = 0; sc < nrMachines; sc++)
        {
            printf("i: %d  e: %d |", solution[sr][sc].initialTime, solution[sr][sc].endTime);
            fprintf(outputFile, "%d  ", solution[sr][sc].initialTime);
        }
        printf("\n");
        fprintf(outputFile, "\n");
    }

    printf("\n");
    printf("********************************");
    printf("\n\nTempo de execucao (s): %f\n", endTime - startTime);
}