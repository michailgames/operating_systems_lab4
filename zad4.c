#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <time.h>

int size_changed = 0;

void terminal_size_changed() {
	size_changed = 1;
}

void end() {
	exit(EXIT_SUCCESS);
}

int main() {
	struct sigaction sigwinch_action;
	sigwinch_action.sa_handler = terminal_size_changed;
	sigaction(SIGWINCH, &sigwinch_action, NULL);
	
	struct sigaction sigint_action;
	sigint_action.sa_handler = end;
	sigaction(SIGINT, &sigint_action, NULL);
	
	terminal_size_changed();
	struct winsize ws;
	struct timespec wait_time;
	while(1) {
		if(size_changed) {
			size_changed = 0;
			if(!isatty(STDIN_FILENO)) {
				exit(EXIT_FAILURE);
			}
			if(ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1) {
				exit(EXIT_FAILURE);
			}
			printf("Terminal size: %d x %d.\n", ws.ws_col, ws.ws_row);
		}
		wait_time.tv_sec = 0;
		wait_time.tv_nsec = 100000;
		nanosleep(&wait_time, NULL);
	}
	return 0;
}
