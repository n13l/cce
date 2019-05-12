#include <sys/compiler.h>
#include <sys/log.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <nids.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <err.h>
#include <libnet.h>
#include <pcap.h>
#include <crypto/hex.h>

#include <mem/stack.h>
#include <net/tcp/proto.h>
#include <net/tls/proto.h>
#include <net/tls/dec.h>
#include <getopt.h>

static const char *opt_file = NULL;
static const char *opts = "vf:k:p:";
static struct option options[] = {
	{"file",   required_argument, 0, 'f' },
	{"verbose", no_argument,      0, 'v' },
	{0,        0,                 0, 0   }
};

static void 
usage()
{
	printf("[options] [file]\n");
	exit(EXIT_FAILURE);
}

static inline void
process_file(void)
{
	int fd = open(opt_file, O_RDONLY | O_NONBLOCK);



	close(fd);
}

int
main(int argc, char *argv[])
{
	int opt = 0, i = 0;
	while ((opt = getopt_long(argc, argv,opts, options, &i )) != -1) {
		switch (opt) {
		case 'f': opt_file = optarg;
			break;
		case 'v': log_verbose++;
			break;
		default: 
			usage(); 
		}
	}

	log_name("readahead");
	log_open("stdout", 0);

	log_setcaps(31);

	process_file();
	log_close();
	return 0;
}
