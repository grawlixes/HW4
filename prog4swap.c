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
	srand(time(NULL));
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

	int cur_size = 1;
	memory[0] = workload[0];

	int * used = malloc(sizeof(int)*memory_size);
	used[0] = 0;

	int hits = 1;

	int i;
	for (i = 1 ; i < NUM_ACCESSES ; i++) {
		int j;
		int chosen = 0;
		for (j = 0 ; j < cur_size ; j++) {
			if (memory[j] == workload[i]) {
				chosen = 1;
				used[j] = 0;
				hits += 1;
			} else {
				used[j] += 1;
			}
		}

		// add new page, compulsory (ignore)
		if (!chosen && memory_size != cur_size) {
			memory[cur_size] = workload[i];
			used[cur_size] = 0;
			cur_size++;
			hits++;
		// add new page, cache full (non-compulsory) 
		} else if (!chosen && memory_size == cur_size) {
			int k;
			int max_index = 0;
			for (k = 1 ; k < memory_size ; k++) {
				if (used[max_index] < used[k]) {
					max_index = k;
				}
			}

			memory[max_index] = workload[i];
			used[max_index] = 0;
		// add no new page, do nothing and move on	
		} else {
			
		}
	}
	free(used);
	return 100*((double) hits)/NUM_ACCESSES;
}

double clock_evict(int * workload, int * memory, int memory_size) {

	int cur_size = 1;
	memory[0] = workload[0];

	int * usage = malloc(sizeof(int)*memory_size);
	usage[0] = 1;

	int hits = 1;

	int i;
	int k = 0;
	for (i = 1 ; i < NUM_ACCESSES ; i++) {
		int j;
		int chosen = 0;
		for (j = 0 ; j < cur_size ; j++) {
			if (memory[j] == workload[i]) {
				chosen = 1;
				usage[j] = 1;
				hits += 1;
				break;
			}
		}

		// add new page, compulsory (ignore)
		if (!chosen && memory_size != cur_size) {
			memory[cur_size] = workload[i];
			usage[cur_size] = 1;
			cur_size++;
			hits++;
		// add new page, cache full (non-compulsory, don't ignore) 
		} else if (!chosen && memory_size == cur_size) {
			while (usage[k] != 0) {
				usage[k] = 0;
				if (k != memory_size-1) {
					k++;
				} else {
					k = 0;
				}
			}

			memory[k] = workload[i];
			usage[k] = 1;

		// add no new page, do nothing and move on	
		} else {
			
		}
	}
	free(usage);
	return 100*((double) hits)/NUM_ACCESSES;
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

double fifo(int *workload, int *memory, int memory_size){
	int memory_used = 0;
	double hit_count = 0;
	int counter = 0;
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
			memory[counter%memory_size] = workload[i];
			counter++;
		}
	}
	return (hit_count/NUM_ACCESSES)*100;
}

double random_evict(int *workload, int *memory, int memory_size){
	unsigned long seed = time(NULL);
	srand(seed);
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
			memory[rand()%memory_size] = workload[i];
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
				if(memory_size < 0 || memory_size > 100 || memory_size%5 != 0){
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

	double hit_rate;
	if (memory_size == 0) {
		hit_rate = 0;

	} else {
		if(strcmp(replacement_policy,"OPT") == 0){
			hit_rate = opt(workload,memory,memory_size);
		}else if(strcmp(replacement_policy,"LRU") == 0){
			hit_rate = lru(memory,memory_size,workload);
		}else if(strcmp(replacement_policy,"FIFO") == 0){
			hit_rate = fifo(workload,memory,memory_size);
		}else if(strcmp(replacement_policy,"Rand") == 0){
			hit_rate = random_evict(workload,memory,memory_size);
		}else{
			hit_rate = clock_evict(workload, memory, memory_size);
		}
	}

	char buf[20];
	sprintf(buf, "%f", hit_rate);
	printf("%s", buf);

	/*
	int i;
	for(i = 0; i < NUM_ACCESSES; ++i){
		printf("%d ",workload[i]);
	puts("");
	}
	*/
	free(memory);
	return 0;
}
