#ifndef ENGINE_PANIC_H_
#define ENGINE_PANIC_H_

#include <stdarg.h>

// Capture File Number, Line Number, and Message.
//   Message should be at most 39 columns by 10 lines
//
//   123456789012345678901234567890123456789
//   2
//   3
//   4
//   5
//   6
//   7
//   8
//   9
//   10
//
//   Total message length including newlines and termination:
//     (39 + 1) * 10 + 1 = 401
//
//   Anything beyond 400 characters will be truncated
#define ENGINE_PANIC(...) EnginePanic(__FILE__, __LINE__, __VA_ARGS__)

#ifdef __cplusplus
extern "C" {
#endif

void EnginePanic(const char *filename, int lineno, const char *format, ...) __attribute__((noreturn));

#ifdef __cplusplus
}
#endif

#endif
