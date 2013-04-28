#ifndef _ERROR_H_
#define _ERROR_H_

extern int compile_warm ();
extern int compile_error (const char *filename, int line, const char *fmt, ...);
extern int compile_error_count ();
extern void compile_error_silent (int silence);

extern int silence, nerrors;

#endif /* _ERROR_H_ */
