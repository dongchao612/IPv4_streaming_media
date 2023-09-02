#include <stdio.h>
#include <stdlib.h>

#include "include/proto.h"
#include "utils/log.h"

int main(int argc, char const *argv[])
{
	print();
	log_info("this is server...");
	return 0;
}
