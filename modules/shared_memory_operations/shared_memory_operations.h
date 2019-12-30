#include <iostream>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

extern unsigned int max_line_length;

/* in_ds */

static int in_ds_id;

static key_t in_ds_key;

static char * in_ds;

/* out_ds */

static int out_ds_id;

static key_t out_ds_key;

static char * out_ds;

/* msg_count */

static int msg_count_id;

static key_t msg_count_key;

static char * msg_count;

/*----------------------------------- Shared Memory Opeations --------------------------------------*/

static int shared_mem_id = 90;

int shared_memory_initializer(int size); /* Initializes a shared memory segment with given size in bytes and returns its ID, or -1 on error */

char * get_pointer_to_shared_memory(int id); /* Returns pointer to pointer of given ID, or NULL on error */

int shared_memory_deinitializer(int shmid); /* Deletes all shared memory segments, return -1 on ERROR, else 0 */

/*--------------------------------------------------------------------------------------------------*/