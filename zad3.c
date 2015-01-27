#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/un.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>
#include <ucontext.h>

#define ERROR(str) { fprintf(stderr, "%s: %s\n", str, strerror(errno)); exit(1); }
#define N 5

#ifdef REG_RIP
#define PC_REG REG_RIP
#elif REG_EIP
#define PC_REG REG_EIP
#endif

#define UNUSED(x) (void)(x)

void str_add_long(char *buffer, long long val, int base) {	
	char buf[32] = {0};
	int i = 30;
	for(; val && i ; --i, val /= (long)base) {
		buf[i] = "0123456789abcdef"[val % base];
	}
	if(i == 30) {
		strcat(buffer, "0");
	}
	else {
		strcat(buffer, buf + i+1);
	}
}

void sigsegv_handler(int sig, siginfo_t *info, void *ctx) {
	UNUSED(sig);
	ucontext_t *context = (ucontext_t *) ctx;
	char buffer[1024];
	
	strcpy(buffer, "-SIGSEGV received-\n");
    
    strcat(buffer, "Memory address causing error: 0x");
    str_add_long(buffer, (long long)info->si_addr, 16);
    strcat(buffer, "\n");
    
    strcat(buffer, "Error code: ");
    str_add_long(buffer, (long long)info->si_code, 10);
    strcat(buffer, "\n");
    
    strcat(buffer, "Address of stack: 0x");
    str_add_long(buffer, (long long) context->uc_stack.ss_sp, 16);
    strcat(buffer, "\n");
    
    strcat(buffer, "Address of instruction causing error: 0x");
    str_add_long(buffer, (long long) context->uc_mcontext.gregs[PC_REG], 16);
    strcat(buffer, "\n");
    
    int n = (int) strlen(buffer);
	if(write(STDERR_FILENO, buffer, n) != n) {
        ERROR("Write to stderr error!");
    }
    exit(EXIT_SUCCESS);
}

int main() {
	size_t page_size = (size_t) sysconf(_SC_PAGESIZE);
	char *buffer;
	buffer = (char *) mmap(0, N * page_size, PROT_READ|PROT_WRITE,
			MAP_ANONYMOUS|MAP_PRIVATE, 0, 0);
	if(buffer == MAP_FAILED) {
		ERROR("mmap error");
	}
	
	strcpy(buffer, "Blah blah blah 1234. ");
	printf("%s\n", buffer);
	mprotect(buffer, N * page_size, PROT_READ);
	
	struct sigaction sigsegv_action;
	sigsegv_action.sa_sigaction = sigsegv_handler;
	sigsegv_action.sa_flags = SA_SIGINFO;
	sigaction(SIGSEGV, &sigsegv_action, NULL);
	
	strcat(buffer, "More blah. ");
	printf("%s\n", buffer);

	return 0;
}
