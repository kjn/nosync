/*-
 * Copyright (c) 2014-2017 Mikolaj Izdebski
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#define _GNU_SOURCE
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>

/* Avoid infinite recursion if called from dlsym(). The implementation
   assumes that open is called before the process goes multi-threaded. */
static int dlsym_pending;

#define OPEN(open)                                              \
                                                                \
int __nosync_ ## open(const char *path, int flags, mode_t mode) \
{                                                               \
  static int (*real)(const char *, int, ...);                   \
  int (*real_copy)(const char *, int, ...)                      \
    = __atomic_load_n(&real, __ATOMIC_RELAXED);                 \
  if (real_copy == NULL) {                                      \
    if (dlsym_pending) {                                        \
      errno = ENOSYS;                                           \
      return -1;                                                \
    }                                                           \
    dlsym_pending = 1;                                          \
    real_copy = dlsym(RTLD_NEXT, #open);                        \
    dlsym_pending = 0;                                          \
    __atomic_store_n(&real, real_copy, __ATOMIC_RELAXED);       \
  }                                                             \
  return real_copy(path, flags & ~(O_SYNC | O_DSYNC), mode);    \
}                                                               \
                                                                \
int open(const char *, int, ...)                                \
  __attribute__((alias("__nosync_" #open)));                    \

OPEN(open)
OPEN(open64)
