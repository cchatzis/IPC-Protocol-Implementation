#include "md5_hashing.h"

using namespace std;

char * md5_hash_string(char * str_2_hash){

    /* Returns an the hashed version of the given string, using md5 */

    /* http://www.askyb.com/cpp/openssl-md5-hashing-example-in-cpp/ */

    unsigned char digest[MD5_DIGEST_LENGTH];

    MD5((unsigned char *) str_2_hash, strlen(str_2_hash), (unsigned char*)&digest );

    char * mdString = (char *) malloc(33);

    for(int i = 0; i < 16; i++) sprintf(&mdString[i*2], "%02x", (unsigned int)digest[i]);

    return mdString;    

}