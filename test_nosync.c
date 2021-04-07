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
#undef NDEBUG

#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// syncfs() is a Linux extension.
// Just expect it to be there, it is provided by nosync on all platforms.
extern  int syncfs(int fd);


int ntest = 0;

#define TEST(MSG) \
    do { printf("Test " MSG "\n"); ntest++; } while (0)


void test_invalid_fd(void)
{
    int invalid_fd;
    int res;

    invalid_fd = open("/etc/issue", O_RDONLY);
    close(invalid_fd);

    TEST("fsync(invalid_fd)");
    res = fsync(invalid_fd);
    assert(res < 0 && errno == EBADF);

    TEST("fdatasync(invalid_fd)");
    res = fdatasync(invalid_fd);
    assert(res < 0 && errno == EBADF);

    TEST("syncfs(invalid_fd)");
    res = syncfs(invalid_fd);
    assert(res < 0 && errno == EBADF);
}


int main(void)
{
    test_invalid_fd();
    printf("\n");
    printf("%i tests passed successfully!\n", ntest);
    return 0;
}
