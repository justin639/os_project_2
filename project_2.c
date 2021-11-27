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
// tie-break rules : smallest #page changes

// inputs
// Page frame, window size
// init page frames are empty
// N(# page per process : max 100) | M(# page frame : max 20; not used in WS)
// W(window size : max 100) | K(page reference string length : max 1000)
// N M W K
// page reference string : page num start from 0
//
// Output
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

// check page fault
int pagefault(int *frame, int next_page, int n_pframe)
{

    int index = linearSearch(-1, n_pframe, next_page, frame);
    if (index == n_pframe)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
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

// title for WS
void printTitle_WS(int n_page)
{
    printf("Time | Ref.string |  #Page ");
    for (int i = 0; i < n_page; i++)
    {
        printf("%-5d", i);
    }
    printf("| P.F.\n");
    return;
}

void printResult(int next_page, int pf_count, int *frame, int n_pframe, char buffer)
{
    printf("%-5d %3d %18d", time_count + 1, next_page, frame[0]);
    for (int i = 1; i < n_pframe; i++)
    {
        if (frame[i] == -1)
        {
            printf("%5s", " ");
        }
        else
        {
            printf("%5d", frame[i]);
        }
    }
    printf("%7c%d\n", buffer, pf_count);
    return;
}

// result for WS
void printResult_WS(int next_page, int pf_count, int *pageCount, int n_page, char buffer)
{
    printf("%-5d %3d %18d", time_count + 1, next_page, pageCount[0]);
    for (int i = 1; i < n_page; i++)
    {
        if (pageCount[i] == 0)
        {
            printf("%5s", " ");
        }
        else
        {
            printf("%5d", pageCount[i]);
        }
    }
    printf("%7c%d\n", buffer, pf_count);
    return;
}

// initialize Page Frame : page number -1 is empty/end/error
void initArray(int *frame, int n_pframe, int value)
{
    for (int i = 0; i < n_pframe + 1; i++)
    {
        frame[i] = value;
    }
}

// MIN : Stack(semi-array)
void MIN(int n_pframe, int ref_length, int *frame, int *rf_String)
{

    initArray(frame, n_pframe, -1);

    time_count = 0;
    pf_count = 0;
    rear = 0;
    int next_page;
    char buffer = ' ';

    printf("============= MIN ==============\n");
    printTitle(n_pframe);

    // for each input
    for (time_count; time_count < ref_length; time_count++)
    {
        next_page = rf_String[time_count];
        buffer = ' ';
        // mem not full
        // page fault
        if (rear < n_pframe)
        {
            pf_count++;
            buffer = 'F';
            frame[rear] = next_page;
            rear++;
        }
        else
        {
            // No P.F. -> do nothing
            if (pagefault(frame, next_page, n_pframe) == TRUE)
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
                    temp_distance = linearSearch(time_count, ref_length, frame[i], rf_String);
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
                frame[target] = next_page;
            }
        }

        printResult(next_page, frame, n_pframe, buffer, pf_count);
    }
    printf("Total Page Fault : %d\n\n", pf_count);
}

// FIFO : Queue
void FIFO(int n_pframe, int ref_length, int *frame, int *rf_String)
{

    initArray(frame, n_pframe, -1);

    time_count = 0;
    pf_count = 0;
    front = n_pframe - 1;
    rear = n_pframe - 1;
    int next_page;
    char buffer = ' ';

    printf("============= FIFO =============\n");
    printTitle(n_pframe);

    // for each input
    for (time_count; time_count < ref_length; time_count++)
    {
        next_page = rf_String[time_count];
        buffer = ' ';
        // add queue if not full
        // page fault
        if (isFull(n_pframe) == FALSE)
        {
            pf_count++;
            buffer = 'F';
            addq(next_page, frame, n_pframe);
        }
        else
        {
            // check page fault
            // P.F. -> delete & add
            // No P.F. -> do nothing
            if (pagefault(frame, next_page, n_pframe) == TRUE)
            {
                pf_count++;
                buffer = 'F';
                deleteq(frame, n_pframe);
                addq(next_page, frame, n_pframe);
            }
        }

        printResult(next_page, frame, n_pframe, buffer, pf_count);
    }
    printf("Total Page Fault : %d\n\n", pf_count);
}

// LRU : Hash table Stack
void LRU(int n_pframe, int ref_length, int *frame, int *rf_String, int *pageCount, int n_page)
{

    initArray(frame, n_pframe, -1);
    initArray(pageCount, n_page, 0);

    time_count = 0;
    pf_count = 0;
    rear = 0;
    int next_page;
    char buffer = ' ';

    printf("============= LRU ==============\n");
    printTitle(n_pframe);

    // for each input
    for (time_count; time_count < ref_length; time_count++)
    {
        next_page = rf_String[time_count];
        buffer = ' ';
        // add to stack if not full
        // update distance(count) of each pages
        // page fault
        if (rear < n_pframe)
        {
            pf_count++;
            buffer = 'F';
            // add
            frame[rear] = next_page;
            rear++;
        }
        else
        {
            // check page fault
            // P.F. -> delete & update
            // delete -> distance = 0 | update
            // No P.F. -> do nothing
            if (pagefault(frame, next_page, n_pframe) == TRUE)
            {
                int max_dist = 0;
                int target;

                // find max count from pageCount array
                for (int i = 0; i < n_pframe; i++)
                {
                    if (pageCount[frame[i]] > max_dist)
                    {
                        target = i;
                        max_dist = pageCount[frame[i]];
                    }
                }
                // update frame
                pageCount[frame[target]] = 0;
                frame[target] = next_page;

                pf_count++;
                buffer = 'F';
            }
            else
            {
                // update counts
                pageCount[next_page] = 0;
            }
        }
        // update counts
        for (int i = 0; i < n_pframe; i++)
        {
            if (frame[i] > -1)
            {
                pageCount[frame[i]]++;
            }
        }

        printResult(next_page, frame, n_pframe, buffer, pf_count);
    }
    printf("Total Page Fault : %d\n\n", pf_count);
}

// LFU : Hash table Stack
void LFU(int n_pframe, int ref_length, int *frame, int *rf_String, int *pageCount, int n_page)
{

    initArray(frame, n_pframe, -1);
    initArray(pageCount, n_page, 0);

    time_count = 0;
    pf_count = 0;
    rear = 0;
    int next_page;
    char buffer = ' ';

    printf("============= LFU ==============\n");
    printTitle(n_pframe);

    // for each input
    for (time_count; time_count < ref_length; time_count++)
    {
        buffer = ' ';
        next_page = rf_String[time_count];
        // add to stack if not full
        // update frequence(count) of each pages
        // page fault
        if (rear < n_pframe)
        {
            pf_count++;
            buffer = 'F';
            // add
            frame[rear] = next_page;
            rear++;
        }
        else
        {
            // check page fault
            // P.F. -> delete & update
            // delete -> distance = 0 | update
            // No P.F. -> do nothing
            if (pagefault(frame, next_page, n_pframe) == TRUE)
            {
                int min_count = MAX_LENGTH;
                int target;

                // find min count(least frequent) from pageCount array
                for (int i = 0; i < n_pframe; i++)
                {
                    if (pageCount[frame[i]] < min_count)
                    {
                        target = i;
                        min_count = pageCount[frame[i]];
                    }
                    else if (pageCount[frame[i]] == min_count)
                    {
                        if (frame[target] > frame[i])
                        {
                            target = i;
                        }
                    }
                }
                // update frame

                // printf("    Page Count:");
                // for(int i=0;i<n_page;i++){
                //     printf("%4d", pageCount[i]);
                // }
                // printf("\n");

                frame[target] = next_page;
                pf_count++;
                buffer = 'F';
            }
        }
        // update counts
        pageCount[next_page]++;

        printResult(next_page, frame, n_pframe, buffer, pf_count);
    }
    printf("Total Page Fault : %d\n\n", pf_count);
}

// WS
void WS(int winSize, int ref_length, int *rf_String, int *pageCount, int n_page)
{

    initArray(pageCount, n_page, 0);

    time_count = 0;
    pf_count = 0;
    int next_page;
    char buffer = ' ';

    printf("============== WS ==============\n");
    printTitle_WS(n_page);

    // for each input
    for (time_count; time_count < ref_length; time_count++)
    {
        buffer = ' ';
        next_page = rf_String[time_count];
        // check page fault(existence)
        if (pageCount[next_page] == 0)
        {
            // P.F. -> pf_count++, buffer = 'F'
            pf_count++;
            buffer = 'F';
        }
        // update validation count (pageCount[i]++)%(winSize + 1)
        // no P.F. -> pageCount[page] = 1;
        // P.F -> pageCount[page] = 1;
        for (int i = 0; i < n_page; i++)
        {
            if (pageCount[i] != 0)
            {
                pageCount[i] = (pageCount[i] + 1) % (winSize + 2);
            }
        }
        pageCount[next_page] = 1;

        printResult_WS(next_page, pageCount, n_page, buffer, pf_count);
    }
    printf("Total Page Fault : %d\n\n", pf_count);
}

int main(int argc, char **argv)
{
    // Local variable
    int n_page, n_pframe, winSize, ref_length;
    int rf_String[MAX_LENGTH + 1] = {0, 1, 2, 3, 2, 3, 4, 5, 4, 1, 3, 4, 3, 4, 5}; // 예시1
    // int rf_String[MAX_LENGTH + 1] = {0, 1, 2, 3, 4, 5, 2, 3, 4, 5, 2, 3, 4, 5, 6, 7, 8, 6, 7, 8}; // 예시2
    // array for distance from current(LRU), current frequency(LFU), validity(WS)
    int pageCount[MAX_PAGE + 1] = {
        0,
    };
    int frame[MAX_FRAME + 1] = {
        0,
    };

    // 예시값
    n_page = 6;
    n_pframe = 3;
    winSize = 3;
    ref_length = 15;

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
    0 0 0 3 3 3 3 3 3 3 3 3 3 3 3 
      1 1 1 1 1 4 5 4 4 4 4 4 4 4
        2 2 2 2 2 2 2 1 1 1 1 1 5
    F F F F     F F F F         F
          0     1 4 5 5         1
0   1 1 1 ' ' ' ' ' ' ' ' ' ' ' '
1     1 1 1 1 1 ' ' ' 2 2 2 2 2 '
2       1 1 2 2 2 2 2 ' ' ' ' ' '
3         1 1 2 2 2 2 2 3 3 4 4 4
4               1 ' 2 2 2 3 3 4 4
5                 1 ' ' ' ' ' ' 2
    Page fault : 9

    WS
    0 1 2 3 2 3 4 5 4 1 3 4 3 4 5
0   1 2 3 4     
1     1 2 3 4         1 2 3 4
2       1 2 1 2 3 4      
3         1 2 1 2 3 4   1 2 1 2 3
4               1 2 1 2 3 1 2 1 2
5                 1 2 3 4       1
    F F F F     F F   F F       F
    Page fault : 9
    */

    /*
    input2.txt
    9 3 3 20
    0 1 2 3 4 5 2 3 4 5 2 3 4 5 6 7 8 6 7 8

    FIFO
    0 1 2 3 4 5 2 3 4 5 2 3 4 5 6 7 8 6 7 8
    F F F F     F F   F F F     F
    Page fault : 10

    input3.txt
    6 4 3 14
    0 1 5 0 3 4 0 1 2 0 3 4 5 3 4

   */

    // MIN : Stack(semi-stack) -> need to chore output format
    MIN(n_pframe, ref_length, frame, rf_String);

    // FIFO : Queue -> need to chore output format
    FIFO(n_pframe, ref_length, frame, rf_String);

    // LRU : Hash table(dist from current)
    LRU(n_pframe, ref_length, frame, rf_String, pageCount, n_page);

    // LFU : Hash table(dist from current)
    LFU(n_pframe, ref_length, frame, rf_String, pageCount, n_page);

    // WS
    WS(winSize, ref_length, rf_String, pageCount, n_page);

    return 0;
}
