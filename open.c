/*-
 * Copyright (c) 2014 Mikolaj Izdebski
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
#include <sys/stat.h>
#include <sys/types.h>

#define OPEN(open)                                              \
int __nosync_ ## open(const char *path, int flags, mode_t mode) \
{                                                               \
  static volatile void *open_ptr;                               \
  static __thread int dlsym_pending;                            \
  int (*real_open)(const char *, int, ...);                     \
                                                                \
  /* Avoid infinite recursion if called from dlsym(). */        \
  if (__builtin_expect(dlsym_pending, 0)) {                     \
    errno = ENOSYS;                                             \
    return -1;                                                  \
  }                                                             \
                                                                \
  real_open = __atomic_load_n(&open_ptr, __ATOMIC_ACQUIRE);     \
                                                                \
  if (__builtin_expect(!real_open, 0)) {                        \
    dlsym_pending = 1;                                          \
    real_open = dlsym(RTLD_NEXT, #open);                        \
    dlsym_pending = 0;                                          \
    __atomic_store_n(&open_ptr, real_open, __ATOMIC_RELEASE);   \
  }                                                             \
                                                                \
  return real_open(path, flags & ~(O_SYNC | O_DSYNC), mode);    \
}                                                               \
                                                                \
int open(const char *, int, ...)                                \
  __attribute__((alias("__nosync_" #open)));                    \

OPEN(open)
OPEN(open64)
