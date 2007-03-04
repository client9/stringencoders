
#include <stdlib.h>

int jabber64_encode(unsigned char *source,
		  size_t sourcelen, char *target, size_t targetlen);

size_t jabber64_decode(char *source, unsigned char *target, size_t targetlen);
