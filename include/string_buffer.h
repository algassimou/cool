#ifndef _STRING_BUFFER_H_
#define _STRING_BUFFER_H_

typedef struct {
  int capacity;
  int len;
  char *data ;
} StringBuffer ;

extern StringBuffer *string_buffer_create ();
extern void string_buffer_del (StringBuffer *buffer);
extern int string_buffer_set(StringBuffer *buffer, char c, int pos);
extern int string_buffer_append (StringBuffer *buffer, char c);
extern int string_buffer_retract (StringBuffer *buffer, int n);
extern char *string_buffer_data(StringBuffer *buffer);

#define STRINGB_CAPACITY 8

#endif /* _STRING_BUFFER_H_ */
