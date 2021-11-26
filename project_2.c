#include <stdio.h>
#include <stdlib.h>

// demand paging system : partition the program into same size blocks(pages)
// PMT
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

#define MAX 200
#define FALSE 0
#define TRUE 1

int main(int argc, char **argv){

    // Local Variables
    int page_num, page_frame_num, window_size, reference_string_len;
    int time_count = 0;

    return 0;
}
