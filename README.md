# OS_Project3
Read writer problem to be solved without starving the readers or the writers.

To Run you only need to have the scenarios.txt file in the same directory. 
Run the command: make 
to build rwmain.
Then run ./rwmain to see the program run. 

Example file input:
rwrrrrwrr
Example output for the file:


 -- Scenario 1 -- 

Reader 0 entering
Writer 0 entering
Reader 1 entering
Reader 2 entering
Reader 3 entering
Reader 4 entering
Writer 1 entering
Reader 5 entering
Reader 6 entering
#########################################
Reader 0 complete
Writer 0 complete
Reader 4 complete
Reader 2 complete
Reader 1 complete
Reader 3 complete
Writer 1 complete
Reader 5 complete
Reader 6 complete

The scenarios.txt file can be modified to test different variations of 
readers and writers entering the system. They will run on a line by line basis.