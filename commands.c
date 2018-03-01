#ifndef COMMANDS
#define COMMANDS

#include <string.h>

#include "time.c"

// handles all common arguments
bool common_args(char * argv[ ], int argc, int * num_integers, int * min_num,
		 int * max_num, char ** output_file_name) {

	int i = 1;
	while (i < argc) {
		if (argv[i][0] == '-' && argv[i][1] == 'u') {
			return false;

		} else if (argv[i][0] == '-' && argv[i][1] == 'n') {

			i += 1;

			// check to make sure the next argument exists
			if (i >= argc) {
				return false;		
	
			// check to make sure the next argument is a number
			} else if (*argv[i] == (char) '-') {
				return false;

			// make sure the number is valid
			} else if (atoi(argv[i]) > 1000000) {
				fprintf(stderr, "Cannot exceed 1,000,000 ints to generate.\n");
				exit(1);			
			}

			*num_integers = atoi(argv[i]);

		} else if (argv[i][0] == '-' && argv[i][1] == 'm') {

			i += 1;

			if (i >= argc) {
				return false;

			} else if (*argv[i] == (char) '-') {
				return false;

			} else if (atoi(argv[i]) < 1) {
				fprintf(stderr, "Cannot have min-int less than 1.\n");
				exit(1);
			}

			*min_num = atoi(argv[i]);

		} else if (argv[i][0] == '-' && argv[i][1] == 'M') {

			i += 1;

			if (i >= argc) {
				return false;

			} else if (*argv[i] == (char) '-') {
				return false;

			} else if (atoi(argv[i]) > 1000000) {
				fprintf(stderr, "Cannot have max-int greater than 1,000,000.\n");
				exit(1);
			} 

			*max_num = atoi(argv[i]);

		} else if (argv[i][0] == '-' && argv[i][1] == 'o') {

			i += 1;

			if (i >= argc) {
				return false;

			} else if (*argv[i] == (char) '-') {
				return false;
			}

			*output_file_name = argv[i];

		} else {
			if (argv[i][0] != '-' || (argv[i][1] != 's' && argv[i][1] !=
			    'i' && argv[i][1] != 'c')) {
				return false;
			} else {
				i += 1;
			}
		}
		i += 1;
	}

	if (*min_num > *max_num) {
		fprintf(stderr, "Cannot have min_int > max_int.\n");
		exit(1);
	}
	
	return true;

}

#endif
