#include <sys/compiler.h>
#include <bsd/list.h>

DEFINE_LIST(list);

struct data {
	struct node n;
	char rnd[64];
};

int main(int argc, char *argv[])
{
	list_for_each(list, it, struct data, n) {
	}
	return 0;
}
