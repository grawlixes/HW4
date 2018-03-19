#include "commands.c"

void print_u();

int generate(int argc, char * argv[ ]) {

	// booleans for our argument options
	bool s = false;

	// default values specified in instructions
	int num_integers = 100;
	int min_num = 1;
	int max_num = 255;
	unsigned long seed = 0;
	char * output_file_name = "";

	// common args (all except seed for generator)
	// if it returns false then there was an input mistake
	// and we have to print out the correct usage
	if (!common_args(argv, argc, &num_integers, &min_num, &max_num, &output_file_name)) {
		print_u();
		exit(1);
	}  


	// iterate through arguments, if any exist
	// use a while loop instead of a for loop
	// so that we can edit i at our leisure
	// i.e. if we have "-n 10" we can read n, 
	// take 10, and disregard 10 in our next loop
	int i = 1;
	while (i < argc) {
		if (argv[i][0] == '-' && argv[i][1] == 's') {

			s = true;

			i += 1;

			if (i >= argc) {
				print_u();
				exit(1);

			} else if (*argv[i] == (char) '-') {
				print_u();
				exit(1);

			}

			seed = strtoul(argv[i], NULL, 10);

		}
 
		i += 1;

	}

	unsigned int x;

	// if we have a seed, use it; otherwise, seed
	// using time of day with time.c function	
	if (s) {

		x = s;

	} else {

		x = get_time_seed();

	}
        
	srand(x);

	FILE * output;	
	bool to_stdout = false;
	if (strlen(output_file_name) == 0) {

		output = stdout;

	} else {

		output = fopen(output_file_name, "w");

	}

	if (num_integers == 0) {
		fprintf(output, "%i\n", 0);
	}

	// writes <num_integers> at the top and
	// then that amount of random numbers
	int j;
	for (j = 0 ; j < num_integers ; j++) {
		int y = (rand() % ((max_num-min_num)+1)) + min_num;

		if (j == 0) {
			fprintf(output, "%i\n", num_integers);

		}

		fprintf(output, "%i\n", y);
	}

	if (strlen(output_file_name) != 0) {
		fclose(output);

	}

	return 0;
}

void print_u() {

	fprintf(stderr, "./prog1generator [-u] [-n <num-integers>] [-m <min-int>] [-M <max-int>] [-s <seed>] [-o <output-file-name>]\n");

}
