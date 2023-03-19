//Галкина Таисия Олеговна, БПИ213
//Вариант №6:
//Разработать программу, заменяющую все согласные буквы в заданной ASCII-строке их ASCII кодами в десятичной системе счисления.

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

const int BUF_SIZE = 5001;

int helpp(char c) {
  if (c == 'b' || c == 'c' || c == 'd' || c == 'f' || c == 'g' || c == 'h' ||
      c == 'j' || c == 'k' || c == 'l' || c == 'm' || c == 'n' || c == 'p' ||
      c == 'q' || c == 'r' || c == 's' || c == 't' || c == 'v' || c == 'w' ||
      c == 'x' || c == 'y' || c == 'z' || c == 'B' || c == 'C' || c == 'D' ||
      c == 'F' || c == 'G' || c == 'H' || c == 'J' || c == 'K' || c == 'L' ||
      c == 'M' || c == 'N' || c == 'P' || c == 'Q' || c == 'R' || c == 'S' ||
      c == 'T' || c == 'V' || c == 'W' || c == 'X' || c == 'Y' || c == 'Z') {
    int k = (int)c;
    return k;
  }
  return -1;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    printf("Not enough args:(\n");
    exit(-1);
  }
  int in_fd, out_fd;
  int pipefd[2];
  pid_t pid;
  char *fifo1 = "fifo1";
  if ((in_fd = open(argv[1], O_RDONLY)) < 0) {
    printf("Error: failed to open input file.\n");
    exit(-1);
  }
  mkfifo(fifo1, 0666);

  char *in_filename = argv[1];
  char *out_filename = argv[2];
  char buf[BUF_SIZE + 1];
  char ans[BUF_SIZE + 1];

  pid = fork();

  if (pid == -1) {
    printf("Error: forking process 1\n");
    return 1;
  } else if (pid == 0) {

    if ((pipefd[1] = open(fifo1, O_WRONLY)) < 0) {
      printf("Error: failed to open input file\n");
      exit(-1);
    }
    ssize_t read_bytes;
    while ((read_bytes = read(in_fd, buf, BUF_SIZE)) > 0) {
      int j = 0;
      for (int i = 0; i < read_bytes; i++) {
        if (helpp(buf[i]) != -1) {
          int k = helpp(buf[i]);
          int fl = 0;
          int pr[3];
          for (int q = 0; q < 3; q++) {
            pr[q] = -1;
          }
          printf("%d ", k);
          while (k > 0) {
            pr[fl] = k % 10;
            k /= 10;
            fl++;
          }
          for (int q = 2; q >= 0; q--) {
            if (pr[q] != -1) {
              ans[j] = pr[q] + '0';
              printf("%c ", ans[j]);
              j++;
            }
          }
        } else {
          ans[j] = buf[i];
          printf("%c ", ans[j]);
          j++;
        }
      }

      ssize_t num_written;
      if ((num_written = write(pipefd[1], ans, j)) < 0) {
        perror("Failed to write to pipe 2");
        exit(1);
      }
    }

    if (close(in_fd) < 0) {
      printf("Can\'t close file\n");
    }
    exit(0);
  } else {
    pipefd[0] = open(fifo1, O_RDONLY);
    if ((out_fd = open(out_filename, O_WRONLY | O_CREAT | O_TRUNC, 0666)) < 0) {
      printf("Error: failed to open output file.\n");
      exit(-1);
    }
    ssize_t read_bytes;
    while ((read_bytes = read(pipefd[0], buf, BUF_SIZE)) > 0) {
      if (write(out_fd, buf, read_bytes) < 0) {
        perror("write");
        exit(1);
      }
    }
    close(pipefd[0]);
    close(out_fd);
    unlink(fifo1);
  }
  return 0;
}
