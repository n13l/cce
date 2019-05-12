#include <iostream>
#include <chrono>
#include <ctime>

#include <sys/compiler.h>
#include <bsd/list.hpp>
#include <bsd/tailq.hpp>
#include <mem/vm.h>

struct slist list();

/* fixed size object */
struct sdata {
	struct snode snode; /* node for sequential access iterations */
	struct tailq tailq; /* node for slot and collisions of hash table */
	char rnd[64];
};

/* fixed power of 2 size object */
struct pdata {
	struct snode snode; /* node for sequential access iterations */
	struct tailq tailq; /* node for slot and collisions of hash table */
	char rnd[64];
	char pad[];
};

/* variable size object */
struct vdata {
	struct snode snode; /* node for sequential access iterations */
	struct tailq tailq; /* node for slot and collisions of hash table */
	unsigned int size;
	char rnd[];
};
 
int main()
{

	/* memory map 1GB memory */	
	void *parts = vm_page_alloc(1<<30);

	std::cout << "Transaction running\n";

	auto beg = std::chrono::system_clock::now();
	
	auto end = std::chrono::system_clock::now();
		     
	std::chrono::duration<double> elapsed = end - beg;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	std::cout << "Transaction elapsed: " << elapsed.count() << "s\n";
	std::cout << "Transaction success\n";

	vm_page_free(parts, 1<<30);

	return 0;
}
