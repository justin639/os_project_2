#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LENGTH 1000
#define MAX_PAGE 100
#define MAX_FRAME 20
#define MAX_WINSIZE 100
#define FALSE 0
#define TRUE 1

// Place Replacement
// Fixed - page fault -> no more page frame left -> replace
// MIN : Minimize page fault frequency
// FIFO : First in First out - queue
// LRU : Least Recently Used - stack
// LFU : Least Frequently Used - stack
//
// Variable - page fault -> page new input(for time interval WS)
// WS Memory Management : after WS page gets out - not page fault!*
//
// Tie-Breaking Rules : LRU - smallest # page replace
//                      LFU - biggest # page replace

// =========== Input ==========
//         N         |           M           |      W      |             K             |
// # Pages(max: 100) | # Pageframes(max: 20) | WS(max:100) | Ref.String Length(max:1000)
// Ref.String(Page: 0~) ...
//
// ========== Output ==========
// MIN
// 메모리 상태 변화 과정 (page fault 발생 위치 표시)
// 총 page fault 횟수
// FIFO / LRU / LFU / WS
// ...

// define Global Variables ======================
// Global Variables
int front, rear; // Queue
int time_count = 0; // Time Count
int pf_count = 0; // Page Fault Count

// define Functions =============================
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

// function for Linear Search in Array
//     if found -> return the index of target 
//     else -> return size of array(index need to be smaller than size)
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

// check Page Fault
//     if target index not found -> return TRUE
//     else -> return FALSE
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

// function for Printing Headers of outputs
void printTitle(int n_pframe)
{
    printf("Time | Ref.string | #Frame ");
    for (int i = 0; i < n_pframe; i++)
    {
        printf("%-5d", i);
    }
    printf("| Page Fault\n");
    return;
}

// function for Printing Headers of outputs for WS
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

// function for Printing Results
void printResult(int next_page, int pf_count, int *frame, int n_pframe, char buffer)
{
    printf("%-5d %3d %18d", time_count + 1, next_page, frame[0]);
    for (int i = 1; i < n_pframe; i++)
    {
        if (frame[i] == -1)
        {
            printf("%5s", "-");
        }
        else
        {
            printf("%5d", frame[i]);
        }
    }
    printf("%7c", buffer);
    if (buffer != ' ')
    {
        printf("%d", pf_count);
    }
    printf("\n");

    return;
}

// function for Printing Results for WS
void printResult_WS(int next_page, int pf_count, int *pageCount, int n_page, char buffer)
{
    printf("%-5d %3d ", time_count + 1, next_page);

    if (pageCount[0] == 0)
    {
        printf("%18c", '-');
    }
    else
    {
        printf("%18d", 0);
    }

    for (int i = 1; i < n_page; i++)
    {
        if (pageCount[i] == 0)
        {
            printf("%5c", '-');
        }
        else
        {
            printf("%5d", i);
        }
    }
    printf("%7c", buffer);
    if (buffer != ' ')
    {
        printf("%d", pf_count);
    }
    printf("\n");

    return;
}

// function for initialize Page Frame
// # Page -1 means empty / end / error
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
    int next_page, target;
    char buffer = ' ';

    printf("MIN\n");
    printTitle(n_pframe);

    // for each input
    for (time_count; time_count < ref_length; time_count++)
    {
        next_page = rf_String[time_count];
        buffer = ' ';

        // No Page Fault -> do nothing
        // Page Fault
        if (pagefault(frame, next_page, n_pframe) == TRUE)
        {
            if (rear < n_pframe) // Mem not full
            {
                frame[rear] = next_page;
                rear++;
            }
            else // Mem full -> replace
            {
                // replace Farthest(MAX Distance)
                // find Replace Target *
                // Local variables
                int target_distance = 0;
                target = 0;
                int temp_distance;

                for (int i = 0; i < n_pframe; i++)
                {   
                    // get Distance of # Page in frame[i]
                    temp_distance = linearSearch(time_count, ref_length, frame[i], rf_String);

                    // find MAX distance
                    // ** Tie-Breaking Rule : replace to Smaller # Page **
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

                // update Target with Current MAX index
                frame[target] = next_page;
            }

            pf_count++;
            buffer = 'F';
        } // end Page Fault
        
        // Print Result
        printResult(next_page, pf_count, frame, n_pframe, buffer);
    }
    printf("Total Page Fault : %d\n\n", pf_count);
} // end MIN

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

        // check Page Fault
        // No Page Fault -> do nothing
        // Page Fault
        if (pagefault(frame, next_page, n_pframe) == TRUE)
        {
            if (isFull(n_pframe) == FALSE) // Mem not Full
            {   
                // just addq
                addq(next_page, frame, n_pframe);
            }
            else // Mem full -> deleteq & addq
            {
                deleteq(frame, n_pframe);
                addq(next_page, frame, n_pframe);
            }

            pf_count++;
            buffer = 'F';
        } // end Page Fault

        // Print Result
        printResult(next_page, pf_count, frame, n_pframe, buffer);
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

    printf("LRU\n");
    printTitle(n_pframe);

    // for each input
    for (time_count; time_count < ref_length; time_count++)
    {
        next_page = rf_String[time_count];
        buffer = ' ';

        // check Page Fault
        // no Page Fault -> update Page counts
        // Page Fault -> replace least recent
        if (pagefault(frame, next_page, n_pframe) == TRUE) // Page Fault
        {
            if (rear < n_pframe) // Mem not Full 
            {
                // add to stack
                frame[rear] = next_page;
                rear++;
            }
            else // Mem full -> replace least recent
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
                // update time stamp 0 -> out-Page stamp = 0, in-Page stamp + 1 
                pageCount[frame[target]] = 0;
                frame[target] = next_page;
            }

            pf_count++;
            buffer = 'F';
        }
        else // no Page Fault
        {
            // update time stamp
            pageCount[next_page] = 0;
        }

        // update time stamping
        // Pages in frame time stamp++
        for (int i = 0; i < n_pframe; i++)
        {
            if (frame[i] > -1)
            {
                pageCount[frame[i]]++;
            }
        }

        // print Result
        printResult(next_page, pf_count, frame, n_pframe, buffer);
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
    int next_page, target;
    char buffer = ' ';

    printf("LFU\n");
    printTitle(n_pframe);

    // for each input
    for (time_count; time_count < ref_length; time_count++)
    {
        buffer = ' ';
        next_page = rf_String[time_count];

        // check page fault
        // no Page Fault -> do nothing
        // Page Fault -> replace least frequent
        // update frequency in last(whether new or not)
        if (pagefault(frame, next_page, n_pframe) == TRUE) // Page Fault
        {
            if (rear < n_pframe) // 
            {
                // add to stack
                frame[rear] = next_page;
                rear++;
            }
            else
            {
                int min_count = MAX_LENGTH;

                // find MIN frequency from pageCount array
                // tie-breaking Rule : bigger # Page is replaced
                for (int i = 0; i < n_pframe; i++)
                {
                    if (pageCount[frame[i]] < min_count)
                    {
                        target = i;
                        min_count = pageCount[frame[i]];
                    }
                    else if (pageCount[frame[i]] == min_count)
                    {
                        if (frame[target] < frame[i])
                        {
                            target = i;
                        }
                    }
                }
                // update frame
                frame[target] = next_page;
            }

            pf_count++;
            buffer = 'F';
        }
        // update frequency for input
        pageCount[next_page]++;

        // print Result
        printResult(next_page, pf_count, frame, n_pframe, buffer);
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

    printf("WS\n");
    printTitle_WS(n_page);

    // for each input
    for (time_count; time_count < ref_length; time_count++)
    {
        buffer = ' ';
        next_page = rf_String[time_count];

        // check page fault(existence)
        if (pageCount[next_page] == 0) // Page Fault
        {
            pf_count++;
            buffer = 'F';
        }

        // update validation count (pageCount[i]+1)%(winSize + 2)
        for (int i = 0; i < n_page; i++)
        {
            if (pageCount[i] != 0) 
            {
                pageCount[i] = (pageCount[i] + 1) % (winSize + 2);
            }
        }

        // whether Page Fault or not, pageCount[in-Page] = 1;
        pageCount[next_page] = 1;

        // print Resault
        printResult_WS(next_page, pf_count, pageCount, n_page, buffer);
    }
    printf("Total Page Fault : %d\n\n", pf_count);
}

int main(int argc, char **argv)
{

    // ---------------- Outline -------------------
    //
    // file read & set initial arrays
    // MIN:
    //     for each input:
    //         page_fault:
    //             Mem_full: add to stack
    //             else: find farthest & replace
    //     print result
    //
    // FIFO:
    //     for each input:
    //         page_fault:
    //             Mem_full: addq
    //             else: deleteq & addq
    //     print result
    //
    // LRU:
    //     for each input:
    //         page_fault:
    //             Mem_full: add to stack
    //             else: find least recent & replace
    //     print result
    //
    // LFU:
    //     for each input:
    //         page_fault:
    //             Mem_full: add to stack
    //             else: find least frequent & replace
    //     print result
    //
    // WS:
    //     for each input:
    //         page_fault:
    //             add to Mem
    //         update based on WS
    //     print result
    // --------------------------------------------

    // Local variable
    int n_page, n_pframe, winSize, ref_length;
    int rf_String[MAX_LENGTH] = { 0, };
    // array for time-stamp(LRU), current frequency(LFU), validity(WS)
    int pageCount[MAX_PAGE] = { 0, };
    int frame[MAX_FRAME] = { 0, };

    if (argc == 2)
    {
        // read File "inpit#.txt"
        FILE *fp = fopen(argv[1], "rb");

        if (fp == NULL)
        {
            printf("Error!\n");
            return 1;
        }
        else
        {
            char temp;
            // set # Pages & # Pageframes & WS & Ref.String Length
            fscanf(fp, "%d %d %d %d", &n_page, &n_pframe, &winSize, &ref_length);

            // set Ref.String
            for (int i = 0; i < ref_length; i++)
            {
                fscanf(fp, "%d ", &rf_String[i]);
            }
        }
        fclose(fp); // end read

        // MIN : Stack(semi-stack)
        MIN(n_pframe, ref_length, frame, rf_String);

        // FIFO : Queue
        FIFO(n_pframe, ref_length, frame, rf_String);

        // LRU : Hash table(time-stamp)
        LRU(n_pframe, ref_length, frame, rf_String, pageCount, n_page);

        // LFU : Hash table(frequency)
        LFU(n_pframe, ref_length, frame, rf_String, pageCount, n_page);

        // WS : Window Size
        WS(winSize, ref_length, rf_String, pageCount, n_page);
    } 
    else // input format worng
    {
        printf("Check enter format: os-sims input#.txt\n");
    }

    return 0;
}
