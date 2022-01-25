#include <unistd.h>

template<size_t N>
inline void write_string_literal(const char (&str)[N]) {
	write(STDOUT_FILENO, str, N - 1);
}


int main() {
	write_string_literal("hello, world\n");
	
	int fd = open();
}
