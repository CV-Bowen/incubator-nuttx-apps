/****************************************************************************
 * apps/examples/hello/hello_main.c
 *
 * Licensed to the Apache Software Foundation (ASF) under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.  The
 * ASF licenses this file to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance with the
 * License.  You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.  See the
 * License for the specific language governing permissions and limitations
 * under the License.
 *
 ****************************************************************************/

/****************************************************************************
 * Included Files
 ****************************************************************************/

#include <nuttx/config.h>

#include <sys/ioctl.h>
#include <stdio.h>
#include <fcntl.h>
#include <errno.h>
#include <debug.h>

/****************************************************************************
 * Public Functions
 ****************************************************************************/

/****************************************************************************
 * hello_main
 ****************************************************************************/

int main(int argc, FAR char *argv[])
{
  int fd;
  ssize_t ret = 0;
  ssize_t ret1;
  char buf[10];

  if (argc != 2)
    {
      printf("Need 2 Arguments\n");
      return ERROR;
    }

  fd = open("/dev/virt_serial0", O_RDWR);
  if (fd < 0)
    {
      printf("Open failed, err=%d\n", errno);
      return ERROR;
    }

  if (*argv[1] == 'r')
    {
      printf("Receive test\n");
      while (1)
        {
          ret = read(fd, buf, 10);
          if (ret < 0)
            {
              printf("Read fail, err=%d\n", errno);
              break;
            }

          lib_dumpbuffer("Read data:", (const uint8_t *)buf, ret);
        }
    }
  else if (*argv[1] == 't')
    {
      printf("Transimit test\n");
      ret = write(fd, "123456789", 10);
      if (ret < 0)
        {
          printf("Write failed, err=%d\n", errno);
        }
    }
  else if (*argv[1] == 'e')
    {
      printf("Echo test\n");
      while (1)
        {
          ret = read(fd, buf, 10);
          if (ret <= 0)
            {
              printf("Read failed, err=%d\n", errno);
              break;
            }

          lib_dumpbuffer("Read data:", (const uint8_t *)buf, ret);

          ret1 = write(fd, buf, ret);
          if (ret1 != ret)
            {
              ret = ret1;
              printf("Write failed, ret1=%ld, err=%d\n", ret1, errno);
              break;
            }
        }
    }

  close(fd);
  printf("Hello, World!!\n");
  return ret;
}
