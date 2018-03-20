#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
//#include <assert.h>
//#include <ctype.h>
#include <unistd.h>

//https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html#Example-of-Getopt

void no_locality(int *workload){
	unsigned long seed = time(NULL);
	srand(seed);
	int i;
	for(i = 0; i < 10000; ++i){
		workload[i] = rand()%99;
	}
}

void looping(int *workload){
	int i;
	for(i = 0; i < 10000; ++i){
		workload[i] = i%50;
	}
}

int main(int argc, char **argv){
	int memory_size = -1;
	char *replacement_policy = NULL;
	char *workload_type = NULL;
	int workload[10000];
	int c;
	while ((c = getopt (argc, argv, "m:r:w:")) != -1)
		switch (c) {
			case 'm':
				memory_size = atoi(optarg);
				if(memory_size <= 0 || memory_size > 100){
					puts("The argument to the \"-m\" option must be at least 1 or at most 100.");
					fputs("./prog4swap [-m size-of-memory] [-r replacement-policy] [-w workload]\n",stderr);
					exit(0);
				}
				break;
			case 'r':
				if(strcmp(optarg,"OPT") == 0 || strcmp(optarg,"LRU") == 0 
						|| strcmp(optarg,"FIFO") == 0 || strcmp(optarg,"Rand") || strcmp(optarg,"Clock") == 0){
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
		//whateverkylenamesthisfunction(workload);
	}else{
		looping(workload);
	}
	/*
	int i;
	for(i = 0; i < 10000; ++i){
		printf("%d ",workload[i]);
	puts("");
	}
	*/
	free(memory);
}
