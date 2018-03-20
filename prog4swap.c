#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <assert.h>
//#include <ctype.h>
#include <unistd.h>

//https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt

#define NUM_ACCESSES 10000

void no_locality(int *workload){
	unsigned long seed = time(NULL);
	srand(seed);
	int i;
	for(i = 0; i < NUM_ACCESSES; ++i){
		workload[i] = rand()%100;
	}
}

void looping(int *workload){
	int i;
	for(i = 0; i < NUM_ACCESSES; ++i){
		workload[i] = i%50;
	}
}

double opt(int *memory,int memory_size, int *workload){
	int i;
	for(i = 0; i < 10000; ++i){
		if(i < memory_size){
			memory[i] = workload[i];
		}
	}
	//printf("")
	return 0;
}

void eighty_twenty(int * workload) {
	int i;
	int count = NUM_ACCESSES*(.8);
	for (i = NUM_ACCESSES-1 ; i >= 0 ; i--) {
		if ((rand()%5 < 4 && count > 0) || (i == count && count > 0)) {
			workload[i] = rand()%20;
			count--;
		} else {
			workload[i] = rand()%80 + 20;
		}
	}
}

double lru(int * memory, int memory_size, int * workload) {
	int i;
	int cur_size = 1;
	memory[0] = workload[0];
	int used[memory_size];
	for (i = 1 ; i < NUM_ACCESSES ; i++) {
		if (cur_size != memory_size) {
			int j;
			int chosen = 0;
			for (j = 0 ; j < cur_size ; j++) {
				if (memory[j] == workload[i]) {
					chosen = 1;
					break;
				}
			}
			if (!chosen) {
				memory[cur_size] = workload[i];
			}
		} else {
			
		}
	}

	return 0;
}

int main(int argc, char **argv){
	int memory_size = -1;
	char *replacement_policy = NULL;
	char *workload_type = NULL;
	int workload[NUM_ACCESSES];
	int c;
	while ((c = getopt (argc, argv, "m:r:w:")) != -1)
		switch (c) {
			case 'm':
				memory_size = atoi(optarg);
				if(memory_size < 5 || memory_size > 100 || memory_size%5 != 0){
					puts("The argument to the \"-m\" option must be 5, 10, 15, ..., 100.");
					fputs("./prog4swap [-m size-of-memory] [-r replacement-policy] [-w workload]\n",stderr);
					exit(0);
				}
				break;
			case 'r':
				if(strcmp(optarg,"OPT") == 0 || strcmp(optarg,"LRU") == 0 
						|| strcmp(optarg,"FIFO") == 0 || strcmp(optarg,"Rand") == 0 || strcmp(optarg,"Clock") == 0){
					replacement_policy = optarg;
				}else{
					puts("Invalid replacement policy");
					fputs("./prog4swap [-m size-of-memory] [-r replacement-policy] [-w workload]\n",stderr);
					exit(0);
				}
				break;
			case 'w':
				if(strcmp(optarg,"No-locality") == 0 || strcmp(optarg,"80-20") == 0 || strcmp(optarg,"Looping") == 0){
					workload_type = optarg;
				}else{
					puts("Invalid workload");
					fputs("./prog4swap [-m size-of-memory] [-r replacement-policy] [-w workload]\n",stderr);
					exit(0);
				}
				break;
			case '?':
				fputs("./prog4swap [-m size-of-memory] [-r replacement-policy] [-w workload]\n",stderr);
				exit(0);
			default:
				abort ();
		}
	if(!replacement_policy){
		puts("No replacement policy entered");
		fputs("./prog4swap [-m size-of-memory] [-r replacement-policy] [-w workload]\n",stderr);
		exit(0);
	}
	if(!workload_type){
		puts("No workload entered");
		fputs("./prog4swap [-m size-of-memory] [-r replacement-policy] [-w workload]\n",stderr);
		exit(0);
	}
	if(memory_size < 0){
		puts("No memory size entered");
		fputs("./prog4swap [-m size-of-memory] [-r replacement-policy] [-w workload]\n",stderr);
		exit(0);
	}

	int *memory = malloc(memory_size*sizeof(int));
	if(strcmp(workload_type,"No-locality") == 0){
		no_locality(workload);
	}else if(strcmp(workload_type,"80-20") == 0){
		eighty_twenty(workload);
	}else{
		looping(workload);
	}

	if(strcmp(replacement_policy,"OPT") == 0){
		//opt(memory,memory_size,workload)
	}else if(strcmp(workload_type,"LRU") == 0){
		lru(memory,memory_size,workload)
	}else if(strcmp(workload_type,"FIFO") == 0){
		//fifo(memory,memory_size,workload)
	}else if(strcmp(workload_type,"Rand") == 0){
		//random_evict(memory,memory_size,workload)
	}else{
		//clock(memory,memory_size,workload)
	}
	/*
	int i;
	for(i = 0; i < NUM_ACCESSES; ++i){
		printf("%d ",workload[i]);
	puts("");
	}
	*/
	free(memory);
}
