#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/uio.h>
#include <errno.h>
#include <sys/un.h>

#define ERROR(str) { fprintf(stderr, "%s: %s\n", str, strerror(errno)); exit(1); }

void print_usage() {
	fprintf(stderr, "Usage: ./zad6 N L mode [buffer_mode]\n");
	fprintf(stderr, "N - number of lines to print (N <= L)\n");
	fprintf(stderr, "L - triagle arm length (L >= 0)\n");
	fprintf(stderr, "mode: 1-write, 2-stdio.h, 3-writev\n");
	fprintf(stderr, "buffer_mode: 0-default, 1-none, 2-line, 3-full\n");
	exit(EXIT_FAILURE);
}

int main(int argc, char *argv[]) {
	if(argc < 4 || argc > 5) {
		print_usage();
	}
	int N = atoi(argv[1]);
	int L = atoi(argv[2]);
	int mode = atoi(argv[3]);
	int buffer_mode = (argc == 5) ? atoi(argv[4]) : 0;
	if(mode < 1 || mode > 3 || buffer_mode < 0 || buffer_mode > 3 ||
			L < 0 || N > L || N < 0) {
		print_usage();
	}
	char *buffer = (char *) malloc(L + 2);
	if(buffer == NULL) ERROR("malloc error");
	for(int i = 0; i < L; i++) buffer[i] = '*';
	buffer[L] = '\n';
	
	char *vbuff;
	int size = 0;
	size = L + 2;
	vbuff = (char *) malloc(size);
	if(vbuff == NULL) ERROR("malloc error");
	int b_mode = _IONBF;
	
	if(buffer_mode == 2) {
		b_mode = _IOLBF;
	}
	else if(buffer_mode == 3) {
		b_mode = _IOLBF;
	}
	if(buffer_mode > 0 && setvbuf(stdout, vbuff, b_mode, size)) ERROR("setvbuf error");
	
	switch(mode) {
	case 1:
		for(int i = 0; i < N; i++) {
			write(STDOUT_FILENO, buffer + i, L-i+1);
		}
		break;
	case 2:
		for(int i = 0; i < N; i++) {
			//for(int j = 0; j < L-i; j++) printf("*");
			fprintf(stdout, "%s", buffer + i);
		}
		break;
	case 3: ; // <- the ; is because labels cannot be followed immediately by declarations
		int iov_max = (int) sysconf(_SC_IOV_MAX);
		struct iovec *vector = (struct iovec *) malloc(N * sizeof(struct iovec));
		if(vector == NULL) ERROR("malloc error");
		int n = 0;
		while(n < N) {
			int i;
			for(i = n; i < N && i < n + iov_max; i++) {
				vector[i].iov_base = buffer + i;
				vector[i].iov_len = L-i+1;
			}
			writev(STDOUT_FILENO, vector + n, i-n);
			n = i;
		}
		free(vector);
		break;
	}
	free(buffer);
	free(vbuff);
}
