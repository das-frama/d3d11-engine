#include "utils/log.h"

/* Procedures. */
void error_print(const char* str) {
	fprintf(stderr, "%s\n", str); fflush(stdout);
	// print to log file.
}

void warning_print(const char* str) {
	fprintf(stdout, "%s\n", str); fflush(stdout);
	// print to log file.
}

void debug_print(const char* str) {
	fprintf(stdout, "%s\n", str); fflush(stdout);
	// print to log file.
}

void log_print(const char* str) {
	fprintf(stdout, "%s\n", str); fflush(stdout);
	// print to log file.
}

/* Functions. */
char* get_asctime() {
	time_t rawtime;
	struct tm tinfo;

	time(&rawtime);
	localtime_s(&tinfo, &rawtime);

	char* out = malloc(sizeof(char) * 32);
	sprintf(out, "%02d.%02d.%d %02d:%02d:%02d", 
		tinfo.tm_mday, tinfo.tm_mon + 1, tinfo.tm_year + 1900, tinfo.tm_hour, tinfo.tm_min, tinfo.tm_sec
	);

	return out;
}
