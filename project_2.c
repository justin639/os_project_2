#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// demand paging system
// page replacement
// MIN : 
// FIFO : first in first out
// LRU : 
// LFU : 
// WS Memory Management : 
// track changes in memory residence set, page fault 

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
// file read & set initial queue
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

// Queue functions
int isEmpty(int front, int rear)
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

int count(int front, int rear, int n_pframe)
{
    if (front > rear)
        return n_pframe - (front - rear);
    return rear - front;
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

void printQ(int *queue, int n_pframe){

    printf("FrameMem: ");
    for(int i =0; i<n_pframe;i++){
        printf("%d ", queue[i]);
    }
    printf("\n");
}

int main(){

    int n_page, n_pframe, n_ws, pr_length;

    int pageCount[MAX_PAGE+1] = {0,};
    int frameMemory[MAX_FRAME+1] = {0,};

    n_pframe = 10; // 예시값

    frameMemory[n_pframe] = -1; // set End of Page Frame : -1 page number is error/end


    front = n_pframe - 1;
    rear = n_pframe - 1;


    // FIFO : Queue
    deleteq(frameMemory,n_pframe);
    printf("front : %d, rear : %d\n",front, rear);
    addq(100,frameMemory,n_pframe);
    printf("front : %d, rear : %d\n",front, rear);
    printQ(frameMemory,n_pframe);
    printf("front : %d, rear : %d\n",front, rear);


    return 0;

}
