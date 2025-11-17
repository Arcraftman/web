#include <iostream>
#include "../../../receptor/src/include/receptor_defs.h"

int main() {
	std::cout << "Web Server Starting ..." << RECEPTOR_OK << std::endl;
	getchar();
	std::cout << "Web Server Stopping...\n";
	return 0;
}