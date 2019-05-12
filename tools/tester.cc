#include <sstream>
#include <iostream>
#include <limits.h>

int
main(int arc, char *argv[])
{
	char buffer[PIPE_BUF];

	std::stringstream ss;
	ss.rdbuf()->pubsetbuf(buffer,sizeof(buffer));

	ss << "test" << "1";

	printf("data: [%s]\n", buffer);

	return 0;
}
