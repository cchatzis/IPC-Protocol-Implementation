#include "input_handling.h"

using namespace std;

int cmd_handling(int argc, char *argv[]){

    /* Handling of Command Line Arguments */

    short int cur_arg = 1;

    bool K_given = 0;

    bool N_given = 0;

    bool text_file_given = 0;

    while( cur_arg < argc ){

        if( strcmp(argv[cur_arg],"-k") == 0 ){ /* Check for number of messages (K) */

            if( cur_arg == (argc - 1) || strcmp(argv[cur_arg+1],"-k") == 0 || strcmp(argv[cur_arg+1],"-n") == 0 || strcmp(argv[cur_arg+1],"-f") == 0 ){
                
                cout << "[ERROR]Please specify a number of messages. (K)" << endl;

                return -1;

            }else{
                
                K_given = 1;

                K = atoi(argv[cur_arg+1]);

            }


        }else if( strcmp(argv[cur_arg],"-n") == 0 ){ /* Check for number of messages (K) */

            if( cur_arg == (argc - 1) || strcmp(argv[cur_arg+1],"-k") == 0 || strcmp(argv[cur_arg+1],"-n") == 0 || strcmp(argv[cur_arg+1],"-f") == 0 ){
                
                cout << "[ERROR]Please specify a number of P processes. (N)" << endl;

                return -1;

            }else{
                
                N_given = 1;

                N = atoi(argv[cur_arg+1]);

            }


        }else if( strcmp(argv[cur_arg],"-i") == 0 ){ /* Check for input file name */

            if( cur_arg == (argc - 1) || strcmp(argv[cur_arg+1],"-k") == 0 || strcmp(argv[cur_arg+1],"-n") == 0 || strcmp(argv[cur_arg+1],"-f") == 0 ){
                
                cout << "[ERROR]Please specify an input file." << endl;

                return -1;

            }else{
                
                text_file_given = 1;

                input_file.open(argv[cur_arg+1]);

                if(!input_file){

                    cout << "[ERROR]Cannot open specified file." << endl;

                    return -1;

                }
            }            



        }else{

            cout << "[ERROR]Invalid Arguments Given. Avalaiable Arguments are:" << endl;

            cout << "-n -k -i" << endl;

            return -1;
        }

        cur_arg = cur_arg + 2;

    }

    if( N_given == 1 && K_given == 1 && text_file_given == 1 ) return 0;
    else{

        cout << "[ERROR]Missing Arguments." << endl;

        return -1;

    }

}

unsigned int get_no_of_lines(void){

    /* Retruns the number of lines in the input_file */

    string str;

    unsigned int lines = 0;

    input_file.clear();
    input_file.seekg(0,ios::beg);

    while( getline(input_file,str) ) ++lines;

    input_file.clear();
    input_file.seekg(0,ios::beg);

    return lines;


}

unsigned int get_max_line_length(void){

    /* Returns max length of line - useful for in-ds */
    
    string str;

    unsigned int max_line_len = 0;

    input_file.clear();
    input_file.seekg(0,ios::beg);

    while( getline(input_file,str) ){

        if( str.length() > max_line_len ) max_line_len = str.length();

    }

    input_file.clear();
    input_file.seekg(0,ios::beg);

    return max_line_len;

} 

void close_file(void){

    /* Closes input_file */

    input_file.close(); 

}