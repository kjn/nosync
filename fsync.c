/*-
 * Copyright (c) 2014-2021 Mikolaj Izdebski
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
#include <errno.h>
#include <pthread.h>

extern int __nosync_check_fd(int fd);

int __nosync_fsync()
{
  pthread_testcancel();
  errno = 0;
  return 0;
}

int msync() __attribute__((alias("__nosync_fsync")));
int sync() __attribute__((alias("__nosync_fsync")));
int sync_file_range() __attribute__((alias("__nosync_fsync")));

int __nosync_fsync_fd(int fd)
{
  if (__nosync_check_fd(fd)) {
    // __nosync_check_fd() sets errno
    return -1;
  }

  return __nosync_fsync();
}

int fsync(int fd) __attribute__((alias("__nosync_fsync_fd")));
int fdatasync(int fd) __attribute__((alias("__nosync_fsync_fd")));
int syncfs(int fd) __attribute__((alias("__nosync_fsync_fd")));
