#include <stdio.h> /*scanf, printf*/
#include <stdlib.h> /*used for atoi*/
#include "q2.h"
#include "q2testCases.c"

int main(int argc, char* argv[])
{
    if(argc <= 1) /*no arguments*/
        runProgram(1, 0);
    else if(argc == 2)
    {
        switch(atoi(argv[1]))
        {
            case 1:
                testCase1(); break;
            default:
                printf("Invalid test case.\n"); 
                printf("Test cases available: 1.\n"); 
                break;
        }
    }
    else if(argc == 3)
    {
        MEM_PART = atoi(argv[1]);
        PROC_PART = atoi(argv[2]);
        runProgram(1, 1);
    }
    else
    {
        printf("Zero, One or Two arguments expected.\n");
        printf("One argument: Test Case Number.\n");
        printf("Two arguments: Number of Memory Partitions and Processes.\n");
        return -1;
    }
    return 0;
}
void getProcessInput(int a[PROC_NUM], const int size)
{
    for(int i = 0; i < size; i++)
    {
        while(printf("\nEnter the size of partition %d in KB:", i)
        && scanf("%d", &a[i]) != 1) //this loops while the input is invalid
        //aka it will ask again
            scanf("%*[^\n]%*c"); /*clear buffer for next scanf*/
    }
}

void printRes(const int m[MEM_NUM], const int p[PROC_NUM], const int f[MEM_NUM])
{
    int count = 0;
    for(int i = 0; i < PROC_PART; i++) notUse[i] = -1;
    
    for(int i = 0; i < MEM_PART; i++)
    {
        if(f[i] == -1) //free
        {
            printf("Partition %-2d of %-3dKB\n", i, m[i]);
        }
        else //not free
        {
            printf("Partition %-2d of %-3dKB contains Processs %-2d of %-3dKB\n", 
            i, m[i], f[i], p[f[i]]);
            notUse[f[i]] = 1;
            count++;
        }
    }
    if (count == PROC_PART)
        printf("All processes have been allocated.\n");
    else
    {
        for(int i = 0; i < PROC_PART; i++)
        {
            if (notUse[i] == -1)
                printf("%-3dKB process %-2d must wait, no memory space for it.\n", p[i], i);
        }
    }
}

void printInput(int p, int m, int pIdx, int mIdx)
{
    printf("%-3dKB process%2d is put in %-3dKB partition %-2d\n", p, pIdx, m, mIdx);
    printf("This leaves us with ");
    for (int i = 0; i < MEM_PART; i++)
        if(inUse[i] == -1)
            printf(" %3dKB ",  memPart[i]);
    printf("\n");
}

//First-fit:  Allocate the first hole that is big enough
void firstFit(const int m[MEM_NUM], const int p[PROC_NUM])
{
    printf("\n\nStep by Step First fit Assignment: \n");
    //set all as -1 as free
    for(int i = 0; i < MEM_PART; i++) inUse[i] = -1;
    
    for(int j = 0; j < PROC_PART; j++)
    {
        //iterate through the memory paritions
        for (int i = 0; i < MEM_PART; i++)
        {
            if(inUse[i] == -1 && m[i] >= p[j])
            {
                inUse[i] = j; //set inUse to the process
                printInput(p[j], m[i], j, i);
                break;
            }
        }
    }
    printf("\nSummary for first fit:\n");
    printRes(m, p, inUse);
}

/*
Best-fit:  Allocate the smallest hole that is big enough; 
must search entire list, unless ordered by size. 
Produces the smallest leftover hole
*/
void bestFit(const int m[MEM_NUM], const int p[PROC_NUM])
{
    printf("\n\nStep by Step Best Fit Assignment: \n");
    int s = -1; // empty, smallest 
    //set all as -1 as free, otherwise set it to the process that is taken by
    for(int i = 0; i < MEM_PART; i++) inUse[i] = -1;

    //iterrate the input processes
    for(int j = 0; j < PROC_PART; j++)
    {
        s = -1; // reset smallest to empty and to use it again
        //iterate through the memory paritions
        for (int i = 0; i < MEM_PART; i++)
        {
            //check if memory partition is taken and mem partition is bigger then input process
            if(inUse[i] == -1 && m[i] >= p[j])
            {
                if(s == -1) s = i; //if s is empty store it into smallest
                if(m[i] < m[s]) // if smallest is smaller then the next mem partition then replaces and store in smallest
                {
                    s = i;
                }
            }
        }
        if(s != -1)
        {   inUse[s] = j; //if smallest not empty then assign the current input process to the smallest mem partiton stored in smallest
            printInput(p[j], m[s], j, s);
        }
    }
    printf("\nSummary for best fit:\n");
    printRes(m, p, inUse);
   
}

/*
Worst-fit:  Allocate the largest hole; 
must also search entire list 
Produces the largest leftover hole
*/
void worstFit(const int m[MEM_NUM], const int p[PROC_NUM])
{
    printf("\n\nStep by Step Worst fit Assignment: \n");
    int worstIndex = -1;
    int biggestGap = 0;
    //set all as -1 as free, otherwise set it to the process that is taken by
    for(int i = 0; i < MEM_PART; i++) inUse[i] = -1;
    
    for(int j = 0; j < PROC_PART; j++)
    {
        worstIndex = -1;
        //iterate through the memory paritions
        for (int i = 0; i < MEM_PART; i++)
        {
            if(inUse[i] == -1 && m[i] >= p[j])
            {
                if(worstIndex == -1)
                {
                    biggestGap = m[i] - p[j];
                    worstIndex = i;
                }
                else if(biggestGap < (m[i] - p[j]))
                {
                    biggestGap = m[i] - p[j];
                    worstIndex = i;
                }
            }
        }
        if(worstIndex != -1)
        {
            inUse[worstIndex] = j;
            printInput(p[j], m[worstIndex], j, worstIndex);
        }
    }
    printf("\nSummary for worst fit:\n");
    printRes(m, p, inUse);
}

int runAll()
{
    firstFit(memPart, procPart);
    bestFit(memPart, procPart);
    worstFit(memPart, procPart);
}

int runAsk(int x)
{
    switch(x)
    {
        case 0: runAll(); break;
        case 1: firstFit(memPart, procPart); break;
        case 2: bestFit(memPart, procPart); break;
        case 3: worstFit(memPart, procPart); break;
        default: printf("Invalid Input\n"); ask(); break;
    }
}

int ask()
{
    int x = 0;
    while(printf("\nEnter 0 for All, 1 for First Fit, 2 for Best Fit and 3 for Worst Fit:")
        && scanf("%d", &x) != 1) //this loops while the input is invalid
        //aka it will ask again
            scanf("%*[^\n]%*c"); /*clear buffer for next scanf*/
    runAsk(x);
}

int runProgram(int getInput, int r)
{
    printf("%d Memory partitions and %d Processes.", MEM_PART, PROC_PART);
    if(getInput == 1)
    {
        printf("\n\nEnter the memory partitions.");
        getProcessInput(memPart, MEM_PART);
        printf("\n\nEnter the process partitions.");
        getProcessInput(procPart, PROC_PART);
        
    }
    if (r == 1) runAll();
    else ask();
}