# My-Alloc  
My-Alloc contains ```alloc``` function that allocates a memory and ```dealloc``` function that frees the memory allocated by the alloc function.  
The functions are implemented and designed to work in Linux environment.

## How to Compile  
1. Download the repository and unzip the file
2. Open the terminal
3. Navigate to the downloaded file
4. In the directory, create a new directoy called "build": ```mkdir build```
5. Navigate into the "build" directory and call command: ```cmake ../```
6. Finally, call command: ```cmake --build .```  

## How to Run - Test Cases
1. Navigate to the newly created "build" file
2. Call command: ```./alloc```

## Observing the "Behind the Scene: Memory Allocation"
1. Navigate to the newly created "build" file
2. Call command: ```cgdb ./alloc```
3. Press 'Enter'
4. To scroll around the test cases, press 'ESC' to get out of typing mode. Press 'k' to move up and 'j' to move down
5. Set a breakpoint (pausing point) at any lines by typing ```b 12``` where 12 is a line number. To switch to typing mode, press 'i'.
6. Type ```run``` to run the test cases.
7. To observe the memory allocation, find a variable and type ```watch p[0]``` where p[0] is a variable.  
   Whenever a change is made to that variable, cgdb will notify.  
   This call must be made when the program is paused at a breakpoint and the variable is declared at that point of paused location. 
