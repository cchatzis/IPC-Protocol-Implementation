CC = g++
modules = modules/input_handling/input_handling.cpp modules/shared_memory_operations/shared_memory_operations.cpp modules/semaphore_operations/semaphore_operations.cpp modules/md5_hashing/md5_hashing.cpp
args = -lcrypto -o 

main1:
	$(CC) main1.cpp $(modules) $(args) main1

main2:
	$(CC) main2.cpp $(modules) $(args) main2

clean:
	if [ -f main1 ]; then rm main1; fi;
	if [ -f main2 ]; then rm main2; fi;