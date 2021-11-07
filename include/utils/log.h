#ifndef MOTOR_LOG_H
#define MOTOR_LOG_H

#include <stdio.h>
#include <string.h>

void error_print(const char* str);
void warning_print(const char* str);
void log_print(const char* str);
void debug_print(const char* str);

char* get_asctime(void);

#define DEBUG_BUFFER_SIZE   2048*4
#define WARNING_BUFFER_SIZE 2048*4
#define LOG_BUFFER_SIZE     2048*4
#define ERROR_BUFFER_SIZE   2048*4

char error_buf[ERROR_BUFFER_SIZE];
char debug_str[ERROR_BUFFER_SIZE];

char warning_buf[WARNING_BUFFER_SIZE];
char warning_str[WARNING_BUFFER_SIZE];

char log_buf[LOG_BUFFER_SIZE];
char log_str[LOG_BUFFER_SIZE];

char debug_buf[DEBUG_BUFFER_SIZE];
char debug_str[DEBUG_BUFFER_SIZE];

#define error(MSG, ...) {\
	snprintf(error_str, ERROR_BUFFER_SIZE-1, "[error] (%s:%s:%i) ", __FILE__, __func__, __LINE__); \
	snprintf(error_buf, ERROR_BUFFER_SIZE-1, MSG, ##__VA_ARGS__); \
	strcat_s(error_str, ERROR_BUFFER_SIZE-1, error_buf); \
	error_print(error_str); \
}

#define warning(MSG, ...) {\
	snprintf(warning_str, WARNING_BUFFER_SIZE-1, "[warning] (%s:%s:%i) ", __FILE__, __func__, __LINE__); \
	snprintf(warning_buf, WARNING_BUFFER_SIZE-1, MSG, ##__VA_ARGS__); \
	strcat_s(warning_str, WARNING_BUFFER_SIZE-1, warning_buf); \
	warning_print(warning_str); \
}

#define log(MSG, ...) {\
	snprintf(log_str, LOG_BUFFER_SIZE-1, "[%s]: ", get_asctime()); \
	snprintf(log_buf, LOG_BUFFER_SIZE-1, MSG, ##__VA_ARGS__); \
	strcat_s(log_str, LOG_BUFFER_SIZE-1, log_buf); \
	log_print(log_str); \
}

#define debug(MSG, ...) { \
	snprintf(debug_str, DEBUG_BUFFER_SIZE-1, "[debug] (%s:%s:%i) ", __FILE__, __func__, __LINE__); \
	snprintf(debug_buf, DEBUG_BUFFER_SIZE-1, MSG, ##__VA_ARGS__); \
	strcat_s(debug_str, DEBUG_BUFFER_SIZE-1, debug_buf); \
	debug_print(debug_str); \
}

#endif // MOTOR_LOG_H
