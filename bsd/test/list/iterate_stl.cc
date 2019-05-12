#include <iostream>
#include <chrono>
#include <ctime>
 
int main()
{
	std::cout << "Transaction running\n";

	auto beg = std::chrono::system_clock::now();
	
	auto end = std::chrono::system_clock::now();
		     
	std::chrono::duration<double> elapsed = end - beg;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	std::cout << "Transaction time elapsed: " << elapsed.count() << "s\n";
	std::cout << "Transaction success\n";

	return 0;
}
