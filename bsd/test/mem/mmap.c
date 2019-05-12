#include <sys/compiler.h>
#include <sys/cpu.h>
#include <sys/mman.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>

static const char *opt_file;

static const char *opts = "f:";
static struct option options[] = {
	{"file",   required_argument, 0, 'f' },
	{"mkhash", no_argument,       0, 'm' },
	{0,        0,                 0, 0   }
};

static void 
print_usage()
{
	printf("Usage: mmap -f [file]\n");
	exit(EXIT_FAILURE);
}

int
main(int argc, char *argv[])
{
	int opt = 0, i = 0;
	while ((opt = getopt_long(argc, argv,opts, options, &i )) != -1) {
		switch (opt) {
		case 'f' : opt_file = optarg;
			break;
		default: 
			print_usage(); 
		}
	}

	return 0;
}
