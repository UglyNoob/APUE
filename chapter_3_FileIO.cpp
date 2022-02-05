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

int my_dup2(int fd, int fd2) {
	close(fd2);
	char buffer[20];
	sprintf(buffer, "/dev/fd/%d", fd);
	fd2 = open(buffer, 0);
	return fd2;
}


int main(int, char **argv) {
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
	/*
	 * When O_APPEND set within flags, lseek, pwrite, pread will lose effect.
	 */
	int fd = open("test.txt", O_WRONLY | O_CREAT | O_EXCL, 0666);
	if(fd == -1) {
		perror(argv[0]);
		return 1;
	}

	off_t offset = lseek(fd, 20, SEEK_SET); // whence can be SEEK_SET SEEK_CUR SEEK_END
	write_string_literal(fd, "hello, world\n");
	printf("%ld bytes wrote to test.txt\n", lseek(fd, 0, SEEK_CUR) - offset);
	// same as: close(STDOUT_FILENO); int dup2licated_fd = fcntl(fd, F_DUPFD, STDOUT_FILENO);
	int dup2licated_fd = dup2(fd, STDOUT_FILENO); // Close STDOUT_FILENO and duplicate fd to STDOUT_FILENO.
	if(dup2licated_fd != STDOUT_FILENO) {
		perror(argv[0]);
	}
	printf("This is written through printf.\n");

	/*
	 * dup dup2: unistd.h
	 */
	// same as: int duplicated_fd = fcntl(fd, F_DUPFD, 0);
	int duplicated_fd = dup(fd); // dup(fd) guaranteed that the new file descriptor returned is lowest-numbered and available.
	write_string_literal(duplicated_fd, "This is written through duplicated_fd.\n");

	/*
	 * fcntl: fcntl.h
	 * int fcntl(int fd, int cmd [, int arg])
	 * F_DUPFD F_DUPFD_CLOEXEC F_GETFD F_SETFD F_GETFL F_SETFL F_SETOWN F_GETOWN
	 */

	/*
	 * ioctl: sys/ioctl.h
	 * int ioctl(int fd, int request [, T *pointer])
	 */

	close(duplicated_fd);
	close(dup2licated_fd);
	close(fd);
	return 0;
}
