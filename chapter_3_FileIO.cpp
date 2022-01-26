#include <cstdio>
#include <fcntl.h>
#include <unistd.h>

template<size_t N>
inline void write_string_literal(int fd, const char (&str)[N]) {
	/*
	 * write: unistd.h
	 */
	write(fd, str, N - 1);
}


int main(int argc, char **argv) {
	/*
	 * STDOUT_FILENO STDERR_FILENO STDIN_FILENO: unistd.h
	 */
	write_string_literal(STDOUT_FILENO, "hello, world\n");
	
	/*
	 * open openat: fcntl.h (returns -1 for errors)
	 * read: unistd.h (returns counts of bytes read, 0 for EOF and -1 for errors)
	 * close lseek: unistd.h
	 *
	 * open and openat options:
	 * O_WRONLY O_RDONLY O_RDWR O_EXEC necesssary
	 * O_APPEND O_CLOEXEC O_CREAT O_DIRECTORORY
	 * O_EXCL(often is with O_CREAT, generates an error if the file already exists.)
	 * O_NOFOLLOW(path must not refer to a symbolic link)
	 * O_NONBLOCK O_SYNC O_DSYNC O_RSYNC O_TRUNC O_TTY_INIT
	 */
	int fd = open("test.txt", O_WRONLY | O_CREAT | O_EXCL, 0600);
	if(fd == -1) {
		perror(argv[0]);
		return 1;
	}

	off_t offset = lseek(fd, 20, SEEK_SET); //whence can be SEEK_SET SEEK_CUR SEEK_END
	write_string_literal(fd, "hello, world\n");
	printf("%ld bytes wrote to test.txt\n", lseek(fd, 0, SEEK_CUR) - offset);

	close(fd);
	return 0;
}
