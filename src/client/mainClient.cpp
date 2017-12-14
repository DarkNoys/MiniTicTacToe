#include "client.h"
#include <string>


int main(int argc, char* argv[]) {
	std::string addr = "127.0.0.1";
	if (argc > 1)
		addr = argv[1];
  Client a(addr);
  a.run();
  return 0;
}
