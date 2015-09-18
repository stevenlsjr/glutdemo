#ifndef __dbg_h__
#define __dbg_h__

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "linmath.h"

#ifdef NDEBUG
#define debug(M, ...)
#else
#define debug(M, ...) fprintf(stderr, "DEBUG %s:%d: " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)
#endif

#define clean_errno() (errno == 0 ? "None" : strerror(errno))

#define log_err(M, ...) fprintf(stderr, "[ERROR] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_warn(M, ...) fprintf(stderr, "[WARN] (%s:%d: errno: %s) " M "\n", __FILE__, __LINE__, clean_errno(), ##__VA_ARGS__)

#define log_info(M, ...) fprintf(stderr, "[INFO] (%s:%d) " M "\n", __FILE__, __LINE__, ##__VA_ARGS__)

#define check(A, M, ...) if(!(A)) { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define sentinel(M, ...)  { log_err(M, ##__VA_ARGS__); errno=0; goto error; }

#define check_mem(A) check((A), "Out of memory.")

#define check_debug(A, M, ...) if(!(A)) { debug(M, ##__VA_ARGS__); errno=0; goto error; }

static inline float hue_to_colorval(float p, float q, float t)
{
  if (t < 0.0f) t += 1.0f;
    if (t > 1.0f) t -= 1.0f;
    if (t < 1.0f / 6.0f) {return p + (q - p) * 6.0f * t;}
    if (t < 1.0f / 2.0f) {return q;}
    if (t < 2.0f / 3.0f) {return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;}


  return p;
}

static inline void hsv_to_rgba(vec4 rgba, float h, float s, float l) 
{
  float r, g, b;
  if (s == 0) {
    // achromatic value
    r = g = b = l;
  } else {
    const float q = (l < 0.5)?
      l * (1 + s):
      l + s - l * s;
    const float p = 2 * l - q;
    const float one_third = 1/3.0;

    r = hue_to_colorval(p, q, h + one_third);
    g = hue_to_colorval(p, q, h);
    b = hue_to_colorval(p, q, h - one_third);

  }

  memcpy(rgba, ((float[4]){r, g, b, 1.0}), sizeof(vec4));
}

#endif
