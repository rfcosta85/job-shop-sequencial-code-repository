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
    fscanf(inputFile, "%d %d", &nrJobs, &nrMachines);
    nrOfOperations = nrJobs * nrMachines;

    Solution solution[nrJobs][nrMachines];

    // Display jobs, machines and operations
    printf("Jobs: %d\n", nrJobs);
    printf("Machines: %d\n", nrMachines);
    printf("Operations: %d\n", nrOfOperations);

    // Start machines
    for (int machineIndex = 0; machineIndex < nrMachines; machineIndex++)
    {
        machines[machineIndex].isFree = true;
        machines[machineIndex].machineNumber = machineIndex;
    }

    // Read input file
    for (int jobIndex = 0; jobIndex < nrJobs; jobIndex++)
    {
        jobs[jobIndex].jobsNumber = jobIndex;

        for (int operationIndex = 0; operationIndex < nrMachines; operationIndex++)
        {
            fscanf(inputFile, "%d %d",
                   &jobs[jobIndex].operations[operationIndex].machine, &jobs[jobIndex].operations[operationIndex].time);
            jobs[jobIndex].operations[operationIndex].job = jobIndex;
        }
    }

    // Display input data
    for (int jobIndex = 0; jobIndex < nrJobs; jobIndex++)
    {
        printf("\n\nJob: %d   | Operations: \n", jobs[jobIndex].jobsNumber);

        for (int operationIndex = 0; operationIndex < nrMachines; operationIndex++)
        {
            printf("J%d M%d T%d | ",
                   jobs[jobIndex].operations[operationIndex].job, jobs[jobIndex].operations[operationIndex].machine,
                   jobs[jobIndex].operations[operationIndex].time);
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
                // to know when the operation in machine will end
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
    for (int jobIndex = 0; jobIndex < nrJobs; jobIndex++)
    {
        printf("Job %d | ", jobIndex);
        for (int machineIndex = 0; machineIndex < nrMachines; machineIndex++)
        {
            printf("startTime: %d  endTime: %d |", solution[jobIndex][machineIndex].initialTime,
                   solution[jobIndex][machineIndex].endTime);
            fprintf(outputFile, "%d  ", solution[jobIndex][machineIndex].initialTime);
        }
        printf("\n");
        fprintf(outputFile, "\n");
    }

    printf("\n");
    printf("********************************");
    printf("\n\nTempo de execucao (s): %f\n", endTime - startTime);
}