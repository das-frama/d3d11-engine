#ifndef MOTOR_UTILS_H
#define MOTOR_UTILS_H

#include <stdint.h>
#include <stdbool.h>

typedef int8_t s8;
typedef uint8_t u8;

typedef int16_t s16;
typedef uint16_t u16;

typedef int32_t s32;
typedef uint32_t u32;

typedef int64_t s64;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

#define MAX_U32 UINT32_MAX
#define MIN_S16 INT16_MIN
#define MAX_S16 INT16_MAX

#define KiB(a) (1024LL * (a))
#define MiB(a) (1024LL * (KiB(a)))
#define GiB(a) (1024LL * (MiB(a)))
#define TiB(a) (1024LL * (GiB(a)))

#define array_count(a) (sizeof(a) / sizeof((a)[0]))

#endif // MOTOR_UTILS_H
