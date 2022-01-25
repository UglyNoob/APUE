#include <errno.h>
#include <cstring>
#include <cstdio>


int main() {
	errno = EEXIST; // errno: errno.h
	perror("Error"); //in cstdio
	printf("Error: %s\n", strerror(errno)); // strerror: cstring
	return 0;
}
