# Final_Project---2-S-Algorithms
A collaborative repository created for the algorithms course in the Software Engineering degree.

This code is a console-based game where the player moves a character horizontally to avoid falling water drops in a grid. It uses windows.h for console manipulation, threading for independent bomb generation, and conio.h for detecting key presses. The player controls movement with keys (a, d, and q to quit), while water drops fall randomly from above, and collision detection ends the game.

The code also features dynamic score tracking, ASCII art displays, and a restart option. It uses a mutex to safely handle shared resources between threads.

Antivirus alerts may occur due to the use of low-level system calls (windows.h), dynamic threading and console manipulation, which are sometimes flagged as suspicious activities. External commands (like curl) and randomness in the output can also raise security concerns.

Compiler: gcc (MinGW.org GCC-6.3.0-1) 6.3.0
SO: Windows 11