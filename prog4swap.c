#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <assert.h>
//#include <ctype.h>
#include <unistd.h>

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

void eighty_twenty(int * workload) {
	int i;
	int count = NUM_ACCESSES*(.8);
	for (i = NUM_ACCESSES ; i >= 0 ; i--) {
		if ((rand()%5 < 4 && count > 0) || (i == count && count > 0)) {
			workload[i] = rand()%20;
			count--;
		} else {
			workload[i] = rand()%80 + 20;
		}
	}
}

int find(int page, int *memory,int memory_size){
	int i;
	for(i = 0; i < memory_size; ++i){
		if(memory[i] == page){
			return 1;
		}
	}
	return 0;
}

double opt(int *workload, int *memory, int memory_size){
	int memory_used = 0;
	double hit_count = 0;
	int i;
	for(i = 0; i < NUM_ACCESSES; ++i){
		if(find(workload[i],memory,memory_size)){
			hit_count++;
			continue;
		}
		if(memory_used < memory_size){
			/*if we still have memory space*/
			memory[memory_used] = workload[i];
			memory_used++;
			/*ignore cold-start/compulsory misses by accumulating the hit count*/
			hit_count++;
		}else{
			int evict_index = -1;
			/*The furthest page in memory we can find so far is the very next one*/
			int furthest= i+1;
			int j;
			for(j = 0; j < memory_size; ++j){
				int k;
				for(k = (i+1); k < NUM_ACCESSES; ++k){
					if(memory[j] == workload[k]){
						/*loop through and see if we can find the page, if it is farther
						 * than the current farthest page, then update the furthest page, and evict the farther page*/
						if(k > furthest){
							furthest = k;
							evict_index = j;
						}
						break;
					}
				}
				if(k == NUM_ACCESSES){
					memory[j] = workload[i];
					break;
				}
			}
			if(memory[j] != workload[i]){
				if(evict_index == -1){
					memory[0] = workload[i];
				}else{
					memory[evict_index] = workload[i];
				}
			}
		}
	}
	return (hit_count/NUM_ACCESSES)*100;
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
		double hit_rate = opt(workload,memory,memory_size);
		printf("%f\n",hit_rate);
	}else if(strcmp(workload_type,"LRU") == 0){
		//lru(memory,memory_size,workload);
	}else if(strcmp(workload_type,"FIFO") == 0){
		//fifo(memory,memory_size,workload);
	}else if(strcmp(workload_type,"Rand") == 0){
		//random_evict(memory,memory_size,workload);
	}else{
		//clock(memory,memory_size,workload);
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
