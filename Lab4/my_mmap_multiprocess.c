//
// Created by modric on 17-11-5.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <time.h>
#include <wait.h>

void data_gen() {
	FILE *f = fopen("in.txt", "w");
	srand(time(0));
	const int lines = 500000;
	const int lenth_per_line = 20;
	for (int i = 0; i < lines; i++) {
		for (int j = 0; j < lenth_per_line; j++) {
			fprintf(f, "%c", (char) (rand()%26 + 'a'));
		}
		fprintf(f, "\n");
	}
	fclose(f);
}

int main() {
	data_gen();
	int file_in = open("in.txt", O_RDONLY);
	int file_out = open("out.txt", O_RDWR | O_CREAT, ~0);

	struct stat stat_file_in;
	fstat(file_in, &stat_file_in);

	lseek(file_out, stat_file_in.st_size - 1, 0);
	write(file_out, "", 1);

	void *src;
	void *dst;
	src = mmap(NULL, stat_file_in.st_size, PROT_READ, MAP_SHARED, file_in, 0);
	dst = mmap(NULL, stat_file_in.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, file_out, 0);

	pid_t pid;
	pid = fork();
	__off_t mid = stat_file_in.st_size/2;

	if (pid > 0) {
		memcpy(dst, src, mid);
	} else {
		memcpy(dst + mid, src + mid, stat_file_in.st_size - mid);
		exit(0);
	}
	waitpid(pid, NULL, 0);

	munmap(src, stat_file_in.st_size);
	close(file_in);
	munmap(dst, stat_file_in.st_size);
	close(file_out);
	return 0;
}
