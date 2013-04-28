#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "compile.h"

int silence, nerrors;

int compile_warm () {
  return 0;
}

int compile_error (const char *filename, int line, const char *fmt, ...) {
  va_list ap;
  if (silence)
    return;

  va_start(ap, fmt);

  if (line > 0) {
    fprintf(stderr, "%s:%d Error ", filename, line);
  } else {
    fprintf(stderr, "%s:EOF Error ", filename);
  }

  vfprintf(stderr, fmt, ap);

  nerrors++ ;
  va_end(ap);
  return 0;
}

int compile_error_count () {
  return nerrors;
}

void compile_error_silent (int s){
  silence = s;
}
