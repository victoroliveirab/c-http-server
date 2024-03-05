#include "internal/log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

void log_message(const char *format, ...) {
  FILE *log_file = fopen("server.log", "a");
  if (log_file == NULL) {
    perror("Error opening log file");
    exit(EXIT_FAILURE);
  }

  // Get current time with milliseconds
  struct timeval tv;
  gettimeofday(&tv, NULL);
  time_t now = tv.tv_sec;
  struct tm *tm_struct = localtime(&now);

  // Create a timestamp string with milliseconds
  char timestamp[26]; // Enough to hold YYYY-MM-DD HH:MM:SS.mmm
  strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", tm_struct);
  int millis = tv.tv_usec / 1000; // Convert microseconds to milliseconds
  fprintf(log_file, "[%s] ", timestamp);

  va_list args;
  va_start(args, format);
  vfprintf(log_file, format,
           args); 
  va_end(args);

  fprintf(log_file, "\n");

  fclose(log_file);
}
