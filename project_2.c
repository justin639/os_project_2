#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// demand paging system : partition the program into same size blocks(pages)
// residence bit ... page frame number
// page replacement
// MIN : Minimize page fault frequency *
// FIFO : first in first out - queue
// LRU : Least Recently Used - stack
// LFU : Least Frequently Used - stack
// process -> pages
// pages -> Memory(page frames)
// page fault -> no more page frame left -> replace (for fixed)
//
// WS Memory Management : working set *
// after window size(WS) page gets out - not page fault
// page fault -> page new input(for time interval WS)
// track changes in memory residence set, page fault
// tie-break rules

// inputs
// Page frame, window size
// init page frames are empty
// N(# page per process : max 100) | M(# page frame : max 20; not used in WS)
// W(window size : max 100) | K(page reference string length : max 1000)
// N M W K
// page reference string : page num start from 0

// output
// MIN
// 메모리 상태 변화 과정 (page fault 발생 위치 표시)
// 총 page fault 횟수
// FIFO
// ...

// ---------- Outline --------------
//
// file read & set initial queue and stacks
//
// show result
// ---------------------------------

#define MAX_LENGTH 1000
#define MAX_PAGE 100
#define MAX_FRAME 20
#define MAX_WINSIZE 100
#define FALSE 0
#define TRUE 1

int front;
int rear;
int time_count = 0;
int pf_count = 0;

// Queue functions
int isEmpty()
{
    if (front == rear)
        return TRUE;
    else
        return FALSE;
}

int isFull(int n_pframe)
{
    int temp = (rear + 1) % n_pframe;
    if (temp == front)
        return TRUE;
    else
        return FALSE;
}

void addq(int PID, int *queue, int n_pframe)
{
    if (isFull(n_pframe))
        printf("Ready Queue is Full.\n");
    else
    {
        rear = (rear + 1) % n_pframe;

        queue[rear] = PID;
    }
}

int deleteq(int *queue, int n_pframe)
{

    if (isEmpty(front, rear))
    {
        printf("Ready Queue is Empty.\n");
        return 0;
    }
    else
    {
        front = (front + 1) % n_pframe;
        return queue[front];
    }
}

// check page fault
int pagefault(int *frame, int next_page, int n_pframe)
{

    for (int i = 0; i < n_pframe; i++)
    {
        if (frame[i] == next_page)
        {
            return FALSE;
        }
    }
    return TRUE;
}

void printTitle(int n_pframe)
{
    printf("Time | Ref.string | #Frame ");
    for (int i = 0; i < n_pframe; i++)
    {
        printf("%-5d", i);
    }
    printf("| P.F.\n");
    return;
}

void printResult(int *rf_String, int *frame, int n_pframe,char buffer){
    printf("%-5d %3d %18d", time_count + 1, rf_String[time_count], frame[0]);
        for (int i = 1; i < n_pframe; i++)
        {
            printf("%5d", frame[i]);
        }
        printf("%7c\n\n", buffer);
    return;
}

// function for linearSearch
int linearSearch(int current, int end, int target, int *rf_String)
{
    for (int i = current + 1; i < end; i++)
    {
        if (rf_String[i] == target)
        {
            return i;
        }
    }
    return end;
}

// initialize Page Frame : page number -1 is empty/end/error
void initFrame(int *frame, int n_pframe)
{
    for (int i = 0; i < n_pframe + 1; i++)
    {
        frame[i] = -1;
    }
}

// MIN : Stack(semi-array)
void MIN(int n_pframe, int pr_length, int *frame, int *rf_String)
{

    initFrame(frame, n_pframe);

    time_count = 0;
    pf_count = 0;
    rear = 0;
    char buffer = ' ';

    printf("============= MIN ==============\n");
    printTitle(n_pframe);

    // for each input
    for (time_count; time_count < pr_length; time_count++)
    {
        buffer = ' ';
        // mem not full
        // page fault
        if (rear < n_pframe)
        {
            pf_count++;
            buffer = 'F';
            frame[rear] = rf_String[time_count];
            rear++;
        }
        else
        {
            // No P.F. -> do nothing
            if (pagefault(frame, rf_String[time_count], n_pframe) == TRUE)
            {
                // printf("%d", time_count+1);

                // check page fault
                // P.F. -> replace farthest
                // find replace target *
                int target_distance = 0;
                int target = 0;
                int temp_distance;

                for (int i = 0; i < n_pframe; i++)
                {
                    temp_distance = linearSearch(time_count, pr_length, frame[i], rf_String);
                    // if distance of #page in frame[i](temp_distance) > current distance
                    // target frame index = i
                    // tie-breaking rule : smaller #page in frame[index]

                    // printf("%20s%d %d", "temp/target: ", temp_distance, target_distance);

                    if (target_distance < temp_distance)
                    {
                        target = i;
                        target_distance = temp_distance;
                    }
                    else if (target_distance == temp_distance)
                    {
                        if (frame[target] > frame[i])
                        {
                            target = i;
                        }
                    }
                }

                // printf("    Target %d\n", target);

                pf_count++;
                buffer = 'F';
                frame[target] = rf_String[time_count];
            }
        }

        printResult(rf_String,frame,n_pframe,buffer);
    }
    printf("Total Page Fault : %d\n", pf_count);
}

// FIFO : Queue
void FIFO(int n_pframe, int pr_length, int *frame, int *rf_String)
{

    initFrame(frame, n_pframe);

    time_count = 0;
    pf_count = 0;
    front = n_pframe - 1;
    rear = n_pframe - 1;
    char buffer = ' ';

    printf("============= FIFO =============\n");
    printTitle(n_pframe);

    // for each input
    for (time_count; time_count < pr_length; time_count++)
    {
        buffer = ' ';
        // add queue if not full
        // page fault
        if (isFull(n_pframe) == FALSE)
        {
            pf_count++;
            buffer = 'F';
            addq(rf_String[time_count], frame, n_pframe);
        }
        else
        {
            // check page fault
            // P.F. -> delete & add
            // No P.F. -> do nothing
            if (pagefault(frame, rf_String[time_count], n_pframe) == TRUE)
            {
                pf_count++;
                buffer = 'F';
                deleteq(frame, n_pframe);
                addq(rf_String[time_count], frame, n_pframe);
            }
        }

        printResult(rf_String,frame,n_pframe,buffer);
    }
    printf("Total Page Fault : %d\n", pf_count);
}

int main(int argc, char **argv)
{
    // Local variable
    int n_page, n_pframe, n_ws, pr_length;
    int rf_String[MAX_LENGTH + 1] = {0, 1, 2, 3, 2, 3, 4, 5, 4, 1, 3, 4, 3, 4, 5}; // 예시1
    // int rf_String[MAX_LENGTH + 1] = {0, 1, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 6, 7, 8, 6, 7, 8}; // 예시2
    // array for distance from current(MIN, LRU), current frequency(LFU), validity(WS)
    int pageCount[MAX_PAGE + 1] = { 0,};
    int frame[MAX_FRAME + 1] = { 0,};

    // 예시값
    n_page = 6;
    n_pframe = 3;
    n_ws = 3;
    pr_length = 15;

    /*
    input1.txt
    6 3 3 15
    0 1 2 3 2 3 4 5 4 1 3 4 3 4 5

    MIN
    0 1 2 3 2 3 4 5 4 1 3 4 3 4 5
    0 0 0 3 3 3 3 5 5 5 5 5 5 5 5
      1 1 1 1 1 1 1 1 1 3 3 3 3 3
        2 2 2 2 4 4 4 4 4 4 4 4 4
    F F F F     F F     F        
          0     2 3     1
    Page fault : 7

    FIFO
    0 1 2 3 2 3 4 5 4 1 3 4 3 4 5
    0 0 0 3 3 3 3 3 3 1 1 1 1 1 5 
      1 1 1 1 1 4 4 4 4 3 3 3 3 3
        2 2 2 2 2 5 5 5 5 4 4 4 4
    F F F F     F F   F F F     F
          0     1 2   3 4 5     1
    Page fault : 10

    LRU
    0 1 2 3 2 3 4 5 4 1 3 4 3 4 5
    0 0 0 3 3 3 3 3 3 1 1 1 1 1 5
      1 1 1 1 1 4 4 4 4 4 4 4 4 4
        2 2 2 2 2 5 5 5 3 3 3 3 3
    F F F F     F F   F F       F
          0     1 2   3 5       1
0   1 2 3       
1     1 2 3 4 5       1 2 3 4 5
2       1 2 1 2 3 
3         1 2 1 2 3 4   1 2 1 2 3
4               1 2 1 2 3 1 2 1 2
5                 1 2 3         1
    Page fault : 9

    LFU
    0 1 2 3 2 3 4 5 4 1 3 4 3 4 5

    WS
    0 1 2 3 2 3 4 5 4 1 3 4 3 4 5

    */

    /*
    input2.txt
    9 3 3 20
    0 1 2 3 4 5 2 3 4 5 2 3 4 5 6 7 8 6 7 8

    FIFO
    0 1 2 3 4 5 2 3 4 5 2 3 4 5 6 7 8 6 7 8
    0 0 0 3 3 3 3 3 3 1 1 1 1 1 5 
      1 1 1 1 1 4 4 4 4 3 3 3 3 3
        2 2 2 2 2 5 5 5 5 4 4 4 4
    F F F F     F F   F F F     F
          0     1 2   3 4 5     1
    Page fault : 10

    input3.txt
    6 4 3 14
    0 1 5 0 3 4 0 1 2 0 3 4 5 3 4

   */

    // MIN : Stack(semi-stack) -> need to chore output format
    MIN(n_pframe, pr_length, frame, rf_String);

    // FIFO : Queue -> need to chore output format
    FIFO(n_pframe, pr_length, frame, rf_String);

    // 

    return 0;
}
