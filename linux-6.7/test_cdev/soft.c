#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int fd;
  char buf1[32] = {0};
  char buf2[32] = "writting......";

  fd = open("/dev/test", O_RDWR);
  if(fd < 0)
  {
    perror("open error \n");
    return fd;
  }
  // read(fd, buf1, sizeof(buf1));
  // printf("get %s\n", buf1);
  buf1[0] = atoi(argv[1]);    //字符串内容从第二位开始
  write(fd, buf1, sizeof(buf1));
  close(fd);
  return 0;
}