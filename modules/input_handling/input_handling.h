#pragma once
#include <iostream>
#include <fstream>
#include <string.h>

using namespace std;

extern unsigned int N; /* Number of P processes */

extern unsigned int K; /* Number of messages to be sent from  */

extern ifstream input_file; /* Input file */

/*------------------------------------ Command Line Handling ---------------------------------------*/

int cmd_handling(int argc, char *argv[]); /* Handling of Command Line Arguments */

unsigned int get_no_of_lines(void); /* Retruns the number of lines in the input_file */

unsigned int get_max_line_length(void); /* Returns max length of line in input_file - useful for in-ds */

void close_file(void); /* Closes input_file */

/*--------------------------------------------------------------------------------------------------*/