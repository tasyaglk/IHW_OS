//Галкина Таисия Олеговна, БПИ213
//Вариант №6:
//Разработать программу, заменяющую все согласные буквы в заданной ASCII-строке их ASCII кодами в десятичной системе счисления.

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <unistd.h>

const int BUF_SIZE = 5001;

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Not enough args:(\n");
    exit(-1);
  }

  int in_fd, out_fd;
  char *fifo1 = "fifo1";

  if ((in_fd = open(fifo1, O_RDONLY)) < 0) {
    printf("Error: failed to open input file\n");
    exit(-1);
  }

  if ((out_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
    printf("Error: failed to open output file\n");
    exit(-1);
  }

  char buf[BUF_SIZE + 1];
  ssize_t read_bytes;

  while ((read_bytes = read(in_fd, buf, BUF_SIZE)) > 0) {
    if (write(out_fd, buf, read_bytes) < 0) {
      perror("write");
      exit(1);
    }
  }
    
    if (close(in_fd) < 0) {
      printf("Error: failed to close fifo\n");
      exit(-1);
    }
    if (close(out_fd) < 0) {
      printf("Error: failed to close file\n");
      exit(-1);
    }
  unlink(fifo1);

  return 0;
}
