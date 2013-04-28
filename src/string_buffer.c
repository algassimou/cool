#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "string_buffer.h"

StringBuffer *string_buffer_create () {
  StringBuffer *s = NULL;

  s = malloc (sizeof(StringBuffer));
  assert (s);
  
  s -> data = malloc (STRINGB_CAPACITY + 1);
  assert (s -> data);
  s -> capacity = STRINGB_CAPACITY;
  s -> len = 0;

  return s;
}

void string_buffer_del (StringBuffer *buffer) {
  if (buffer) {
    if (buffer -> data) {
      free (buffer -> data);
    }
    free (buffer);
  }
}

int string_buffer_append(StringBuffer *buffer, char c) {
  assert(buffer);
  assert (buffer-> data);

  if (buffer-> len == buffer -> capacity) {
    // realloc data
    buffer -> data = realloc (buffer-> data, buffer -> capacity + STRINGB_CAPACITY + 1);
    assert (buffer-> data);
    buffer -> capacity = buffer-> capacity + STRINGB_CAPACITY;
  }
  
  buffer -> data [buffer -> len] = c;
  (buffer -> len)++;
  return 0;
}

int string_buffer_set(StringBuffer *buffer, char c, int pos) {
  assert (buffer);
  assert(buffer -> data);
  assert (pos >= 0 && pos <= buffer -> len);

  buffer -> data [pos] = c;
  return 0;
}

char *string_buffer_look_data(StringBuffer *buffer) {
  assert (buffer);
  assert(buffer -> data);

  string_buffer_append(buffer, 0);
  string_buffer_retract(buffer, 1);
  return buffer -> data;
}


char *string_buffer_get_data(StringBuffer *buffer) {
  assert (buffer);
  assert(buffer -> data);

  char *res = strndup (buffer -> data, buffer -> len + 1);
  assert (res);

  res [buffer -> len] = '\0';

  return res ;
}

int string_buffer_retract (StringBuffer *buffer, int n) {
  assert (buffer);
  assert (buffer -> data);
  assert (buffer -> len >= n);

  buffer -> len = buffer -> len - n;
  return 0;
}
