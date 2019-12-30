#include "modules/input_handling/input_handling.h"
#include "modules/shared_memory_operations/shared_memory_operations.h"
#include "modules/semaphore_operations/semaphore_operations.h"
#include "modules/md5_hashing/md5_hashing.h"
#include <unistd.h>
#include <sys/wait.h>

using namespace std;

unsigned int N=0; /* Number of P processes */

unsigned int K=0; /* Number of messages to be sent */

ifstream input_file; /* Input file */

unsigned int max_line_length=0; /* Max line length of input file */

unsigned int total_input_file_lines=0; /* Lines of total input file */

int main(int argc, char *argv[]){

    /* Command Line Input Handling */

	int input = cmd_handling(argc,argv);

	if( input == -1 ){

		std::cout << "The program will now exit." << endl;

		return -1;

	}

    /* Seed random number generator */

    srand(time(NULL));

    /* Get max_line_length from input_file - this, increased by sizeof(int) will be the size of in-ds */

    max_line_length = get_max_line_length();

    /* Get the number of lines the input file has, will be used as upper bound to random line number generator */

    total_input_file_lines = get_no_of_lines();

    /* Shared Memory Initialization */

    int in_ds_id = shared_memory_initializer( 2 * sizeof(int) + K * (sizeof(int) + max_line_length) ); /* in_ds shared segment ID */

    int out_ds_id = shared_memory_initializer( 2 * sizeof(int) + K * ( 32 + sizeof(int) ) ); /* out_ds shared segment ID */

    /* Secure pointers to shared memory segments */

    char * in_ds; /* Pointer to in_ds shared memory segment */

    char * out_ds; /* Pointer to out_ds shared memory segment */

    in_ds = get_pointer_to_shared_memory(in_ds_id);

    out_ds = get_pointer_to_shared_memory(out_ds_id);

    /* Intiialize in_ds shared memory */

    const int zero = 0;

    memcpy(in_ds,&zero,sizeof(int));

    memcpy(in_ds+sizeof(int),&zero,sizeof(int));

    /* Intiialize out_ds shared memory */

    memcpy(out_ds,&zero,sizeof(int));  

    memcpy(out_ds+sizeof(int),&zero,sizeof(int));

    /* Semaphore Initialization */

    /* in_ds */

    int in_ds_mutex = semaphore_create();

    if( semaphore_init_1(in_ds_mutex) == -1 ) return -1;

    /* out_ds */

    int out_ds_mutex = semaphore_create();

    if( semaphore_init_1(out_ds_mutex) == -1 ) return -1;

    /* Now lets fork the C process */

    bool is_c_process = 0;

    int main_pid = getpid();

    int pid_match = 0;

    pid_t * children_pids = (pid_t * ) malloc( (N+1) * sizeof(pid_t) );

    children_pids[N] = fork();

    if( getpid() != main_pid ){

        /* Only C processes enter this section */

        pid_match = -1;

        std::cout << "[MESSAGE]Just forked a C process with pid: " << getpid() << endl;

        while(1){

            is_c_process = 1;

            /* Can i receive another message? */

            semaphore_p(out_ds_mutex);

            int sent_msgs;

            memcpy(&sent_msgs,out_ds,sizeof(int));

            semaphore_v(out_ds_mutex);

            if( sent_msgs == K ) break;

            /* Preparations to receive message from P processes */
        
            int sender_pid;

            char * str = (char *) malloc(max_line_length+1);

            memset(str,'\0',max_line_length+1);

            /* Compare values of read_msgs and sent_msgs to find out if all sent messages have been received */

            int read_msgs_in_ds,sent_msgs_in_ds;

            while(1){

                semaphore_p(in_ds_mutex);

                memcpy(&sent_msgs_in_ds,in_ds,sizeof(int));

                memcpy(&read_msgs_in_ds,in_ds+sizeof(int),sizeof(int));

                if( sent_msgs_in_ds > read_msgs_in_ds ) break;

                semaphore_v(in_ds_mutex);

            }

            memcpy(&sender_pid,in_ds + 2*sizeof(int) + read_msgs_in_ds * (sizeof(int) + max_line_length ) ,sizeof(int));

            // std::std::cout << "[MESSAGE]C process (" << getpid() << ") received message from " << sender_pid << "." << endl;

            memcpy(str,in_ds + 3*sizeof(int) + read_msgs_in_ds * (sizeof(int) + max_line_length )  ,max_line_length);

            read_msgs_in_ds++;

            memcpy(in_ds+sizeof(int),&read_msgs_in_ds,sizeof(int));

            semaphore_v(in_ds_mutex);

            /* Hash Received Message */

            char * hashed_str = md5_hash_string(str);

            /* Send Response to all P processes using out_ds */

            semaphore_p(out_ds_mutex);

            memcpy(out_ds + 2 * sizeof(int) + sent_msgs * (sizeof(int) + 32 ),&sender_pid,sizeof(int));

            memcpy(out_ds + 3 * sizeof(int) + sent_msgs * (sizeof(int) + 32 ),hashed_str,strlen(hashed_str));

            sent_msgs++;

            memcpy(out_ds,&sent_msgs,sizeof(int));
            
            semaphore_v(out_ds_mutex);

            /* Free dynamically allocated memory */

            std::free(str);

            std::free(hashed_str);

        }

    }else{

        /* Now lets fork the P processes (specifically, N of them) */

        int i;
        
        for(i=0;i<N;i++){

            children_pids[i] = fork();
            
            if( getpid() != main_pid ) break;

        }
        
    }

    if( getpid() != main_pid && is_c_process == 0 ){

        /* Only P processes enter this section */

        std::cout << "[MESSAGE]Just forked a P process with pid: " << getpid() << endl;

        while(1){

            /* Can i send a message? Lets check the msg_count shared memory and find out */

            semaphore_p(in_ds_mutex);

            int sent_msgs;

            memcpy(&sent_msgs,in_ds,sizeof(int));

            if( sent_msgs >= K ){
                
                semaphore_v(in_ds_mutex);

                break;
                
            } 

            /* Message Preparations - get pid and get a random line from input text */
            
            int mypid = getpid();

            input_file.clear();
            input_file.seekg(0,ios::beg);

            string str;

            std::getline(input_file,str);

            int seed = rand() % total_input_file_lines;

            for(int j=0;j<seed;j++) std::getline(input_file,str);
            
            char * str_2_send = (char *) malloc(str.length());

            memcpy(str_2_send,str.c_str(),str.length());

            /* Send the message */

            memcpy(in_ds + 2 * sizeof(int) + sent_msgs * ( sizeof(int) + max_line_length ),&mypid,sizeof(int));

            memcpy(in_ds + 3 * sizeof(int) + sent_msgs * ( sizeof(int) + max_line_length ),str_2_send,max_line_length);
            
            sent_msgs++;

            memcpy(in_ds,&sent_msgs,sizeof(int));

            semaphore_v(in_ds_mutex);

            /* Wait for C Process Response from out_ds */

            char * hashed_str = (char *) malloc(33);

            memset(hashed_str,'\0',33);

            int msgs_sent_out_ds;

            int prev_pid_match = pid_match;

            int msg_pid;

            while( pid_match == prev_pid_match ){

                sleep(1);       

                semaphore_p(out_ds_mutex);

                memcpy(&msgs_sent_out_ds,out_ds,sizeof(int));

                for(int l=0;l<msgs_sent_out_ds;l++){
                    
                    memcpy(&msg_pid, out_ds + 2*sizeof(int) + l * (sizeof(int) + 32),sizeof(int) );

                    memcpy(hashed_str,out_ds + 3*sizeof(int) + l * (sizeof(int) + 32),32);

                    if( msg_pid == getpid() && msg_pid != 0 ){ /* Is the message in out_ds a message sent by this process? */

                        pid_match++;

                        memcpy(out_ds + 2*sizeof(int) + l * (sizeof(int) + 32),&zero,sizeof(int));


                    }else{ /* Turns out its not, restore semaphore state */

                        if( msg_pid != 0 ) std::cout << "[MESSAGE]Process with pid " << getpid() << " found a message from " << msg_pid << " in out-ds: " << hashed_str << endl;
                        

                    }

                    semaphore_v(out_ds_mutex);
                }


            }

            std::cout << "[SUCCESS]pid_match happened: " << hashed_str << " (" << msg_pid << ")" << endl;

            /* Free dynamically allocated Memory */

            std::free(str_2_send);

            std::free(hashed_str);

        }

    }


    if( getpid() == main_pid ){

        /* Wait for all children processes to finish executing */

        unsigned int total_pid_matches = 0;

        int status=0;

        // while((wpid = wait(&status) > 0 ));

        /* Wait for C process to exit */

        waitpid(children_pids[N],&status,0);

        /* Wait for all the P processes to exit */

        for(int j=0;j<N;j++){

            waitpid(children_pids[j],&status,0);

            if( WIFEXITED(status) ){

                int returned = WEXITSTATUS(status);

                total_pid_matches = total_pid_matches + returned;

            }

        }

        /* Print statistics */

        std::cout << endl;

        std::cout << "===========================STATISTICS===========================" << endl;

        std::cout << "Number of P Processes: " << N << " PIDs: [";

        for(int j=0;j<N;j++) std::cout << " " << children_pids[j] << " ";

        std::cout << "]" << endl;

        std::cout << "Total Number of messages sent: " << K << endl;

        std::cout << "Total number of pid_matches: " << total_pid_matches << endl;

        std::cout << "================================================================" << endl;

        std::cout << endl;

        /* Clear shared Memory Segments */

        shared_memory_deinitializer(in_ds_id);

        shared_memory_deinitializer(out_ds_id);

        /* Clear Semaphores */

        /* in_ds */

        semaphore_delete(in_ds_mutex);

        /* out_ds */

        semaphore_delete(out_ds_mutex);

        /* msg_count */

        /* Close Input File */

        close_file();

    }

    /* Deallocate Memory */

    free(children_pids);   

    std::cout << "[MESSAGE]Process with pid " << getpid() << " and pid_match " << pid_match << " just exited." << endl;

    return pid_match;
}