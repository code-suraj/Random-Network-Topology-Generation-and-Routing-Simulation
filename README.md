The WPS.cpp file can be run via terminal.

Once we run the file using "g++ -o a WPS1.cpp" command, a.exe file will be generated in the same folder, then in the terminal we can run the a.exe file using "./a" command.

So, the sequence of commands are : 
 1. g++ -o a WPS.cpp
 2. ./a
 
 Note : The terminal have to be opened in the same folder as of WPS.cpp file
 
 
 Once, we run the executable file, we will be asked to "Enter Mode (User/Random): ", where in we have to enter whcihever mode we want. The input is case insensitive, so for chooseing user mode or random mode we can enter in uppercase or lowercase or mixed charecters. 
 
 Note : If entered other than user or random keyword, then the process exits, saying : "Please choose from these options: User / Random only"
 
 
 - If user mode is choosen, the input will be taken from input.txt file which have to be present in the same directory as of the WPS.cpp file.
 - If random mode is choosen, the input nodes will be generated randomly
 
 And after the input mode is choosen, the cluster heads, and node-cluster assignments will be done and will be printed on the terminal and also written to 'network.txt' file (whose previous data will be erased first, if 'network.txt' doesnot exist in the current working directory then the file will be created first)
 
 - Now, user will be asked to enter source_node_id and destination_node_id and if they are valid, then the porgram will print the route from source id's node to destination id's node using the greedy routing algorithm, and says no path exists, if there is no path.
 
 -Now the user will be asked to choose whether they want to quit, if yes/quit/y/q (case insensitive), then the program exits, or else the user will be asked for valid source_id and destination_id, and prints the route if exists, this repeats on untill the user chooses to quit.
 