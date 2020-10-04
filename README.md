# IPC Project - Cycle Repairing.

Imagine there exists a cycle repairing shop. This shop has some weird rules for the cycles coming 
here to take service. All the cycles that want servicing from this shop must be present at the time 
of opening of the shop. There are S number of servicemen in the shop. They work sequentially one 
after another; i.e., serviceman 1 first inspects a cycle, then the cycle is passed to serviceman 2 and 
so on. After serviceman 1 finishes inspecting a cycle and this cycle is passed to serviceman 2;
another cycle from the remaining cycles starts taking service from serviceman 1 and so on.


After a cycle’s servicing is finished (i.e., it took service from all S servicemen), owner of the cycle 
must go to a room for the payment. However, that room can only contain C number of customers. 
After a cyclist is done with his payment, he will go out of the shop. However, he must return 
through the path that is being used by the servicemen for servicing the cycles. But, this path 
is so narrow that only one directional movement is possible; i.e., when any cycle is being serviced 
by any one of the servicemen, no cyclist can return through this path. Similarly, while a cyclist is
returning through this path for departure, no new cycle can enter this path for starting its service.

Our task was to simulate the above mentioned scenario using the concept of Inter Process Communication of Operating System. This was a mini project of Operating System Sessional Course.

## Tools and Languages

**Language Used:** C++

**Operating System Used**: Linux( *Ubuntu 18.04* )

## Files

**1605079.cpp** : This is the main file of the project. The number indicates my student ID.

**IPC Assignment.pdf** : This is the description of the project.

**checker_Redwan.cpp**: A checker to check the correctness written by one of my friend.

**output.txt**: A sample output file indicating what the output file should look like.

**script.sh**: A shell script to run the main **.cpp** file. This generates an output executable with the same name.

**stress.sh**: A shell script to test the main **.cpp** file with the **checker_Redwan.cpp** file.

**usage.txt**: A text file providing useful instruction on how to run the **stress.sh** file for testing purposes.



## Instruction to Run the file

### To Run and Generate Output

To run the main **.cpp** file, simply run the **script.sh** shell script file and it will automatically generate an executable for you and redirect the output to a **.txt** file with the same name as the **.cpp** file. If you want to compile the main **.cpp** file and see the output on the console, you open up your terminal and type in the following command:

***g++ -D _REENTRANT 1605079.cpp -o 1605079 -lpthread***

This will generate an executable, then just run the executable with:

***./1605079***

### To test the file

All the instruction to test the file has been provided in the **usage.txt** file. Follow the instruction carefully and test for any errors or bugs in the the main file. 

