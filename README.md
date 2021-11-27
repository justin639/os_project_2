# Project02 for OS Lecture   
## os_project_2
### Examples
#### Example1
>input1.txt   

    6 3 3 15   
    0 1 2 3 2 3 4 5 4 1 3 4 3 4 5   

    1. MIN
        0 1 2 3 2 3 4 5 4 1 3 4 3 4 5
        0 0 0 3 3 3 3 5 5 5 5 5 5 5 5
          1 1 1 1 1 1 1 1 1 3 3 3 3 3
            2 2 2 2 4 4 4 4 4 4 4 4 4
        F F F F     F F     F        
              0     2 3     1
        Page fault : 7

    2. FIFO
        0 1 2 3 2 3 4 5 4 1 3 4 3 4 5
        0 0 0 3 3 3 3 3 3 1 1 1 1 1 5 
          1 1 1 1 1 4 4 4 4 3 3 3 3 3
            2 2 2 2 2 5 5 5 5 4 4 4 4
        F F F F     F F   F F F     F
              0     1 2   3 4 5     1
        Page fault : 10

    3. LRU
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

    4. LFU
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

    5. WS
        0 1 2 3 2 3 4 5 4 1 3 4 3 4 5
    0   1 2 3 4     
    1     1 2 3 4         1 2 3 4
    2       1 2 1 2 3 4      
    3         1 2 1 2 3 4   1 2 1 2 3
    4               1 2 1 2 3 1 2 1 2
    5                 1 2 3 4       1
        F F F F     F F   F F       F
        Page fault : 9
    
>input2.txt
    9 3 3 20
    0 1 2 3 4 5 2 3 4 5 2 3 4 5 6 7 8 6 7 8
M   F F F F
F   F F F F     F F   F F F     F           10
LRU F F F F
LFU F F F F
WS  F F F F F F

    input3.txt
    6 4 3 14
    0 1 5 0 3 4 0 1 0 3 4 5 3 4
M   F F F   F F           F     6 
F   F F F   F F F F       F F F 10
LRU F F F   F F   F       F     7
LFU F F F   F F   F       F     7
WS  F F F   F F   F   F F F     9

   */