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
  int in_fd, out_fd, fd;
  pid_t pid1, pid2, pid3;
  char *fifo1 = "fifo1";
  char *fifo2 = "fifo2";

  mkfifo(fifo1, 0666);
  mkfifo(fifo2, 0666);

  char *in_filename = argv[1];
  char *out_filename = argv[2];
  char buf[BUF_SIZE + 1];
  char ans[BUF_SIZE + 1];

  pid1 = fork();

  if (pid1 == -1) {
    printf("Error: forking process 1\n");
    return 1;
  } else if (pid1 == 0) {

    in_fd = open(in_filename, O_RDONLY);
    if (in_fd == -1) {
      printf("Error: failed to open input file\n");
      exit(-1);
    }
    fd = open(fifo1, O_WRONLY | O_TRUNC);
    if (fd < 0) {
      printf("Error: failed to open fifo1\n");
      exit(-1);
    }
    ssize_t read_bytes;
    do {
      read_bytes = read(in_fd, buf, BUF_SIZE);
      if (read_bytes == -1) {
        printf("Error: failed to read file\n");
        exit(-1);
      }
      buf[read_bytes] = '\0';
      int size = write(fd, buf, read_bytes);
      if (size == -1) {
        printf("Error: failed to write file\n");
        exit(-1);
      }
    } while (read_bytes == BUF_SIZE);

    if (close(in_fd) < 0) {
      printf("Error: failed to close file\n");
    }
    if (close(fd) < 0) {
      printf("Error: failed to close file\n");
    }
    exit(0);
  }

  pid2 = fork();
  if (pid2 == -1) {
    perror("Error: forking process 2\n");
    exit(1);
  } else if (pid2 == 0) {
    in_fd = open(fifo1, O_RDONLY);
    fd = open(fifo2, O_WRONLY | O_TRUNC);
    if (in_fd < 0) {
      printf("Error: failed to open fifo1\n");
      exit(-1);
    }
    if (fd < 0) {
      printf("Error: failed to open fifo2\n");
      exit(-1);
    }
    char ans[BUF_SIZE];
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
          while (k > 0) {
            pr[fl] = k % 10;
            k /= 10;
            fl++;
          }
          for (int q = 2; q >= 0; q--) {
            if (pr[q] != -1) {
              ans[j] = pr[q] + '0';
              j++;
            }
          }
        } else {
          ans[j] = buf[i];
          j++;
        }
      }
      if (write(fd, ans, j) == -1) {
        perror("Failed to write to fifo2\n");
        exit(1);
      }
    }
    if (close(in_fd) < 0) {
      printf("Error: failed to close file\n");
      exit(-1);
    }
    if (close(fd) < 0) {
      printf("Error: failed to close file\n");
      exit(-1);
    }
    exit(0);
  }

  pid3 = fork();
  if (pid3 == -1) {
    printf("Error: forking process 3\n");
    exit(-1);
  } else if (pid3 == 0) {
    fd = open(fifo2, O_RDONLY);
    if (fd < 0) {
      printf("Error: failed to open fifo2\n");
      exit(-1);
    }
    out_fd = open(out_filename, O_WRONLY | O_CREAT, 0666);
    if (out_fd == -1) {
      printf("Error: failed to open output file\n");
      exit(-1);
    }
    ssize_t read_bytes = read(fd, ans, sizeof(ans));
    if (read_bytes == -1) {
      printf("Error: failed to read file\n");
      exit(-1);
    }

    if (write(out_fd, ans, read_bytes) == -1) {
      perror("Error: failed to write to pipe 2\n");
      exit(1);
    }

    if (close(out_fd) < 0) {
        perror("Error: failed to close file\n");
        exit(1);
    }
    if (close(fd) < 0) {
        perror("Error: failed to close fifo2\n");
        exit(1);
    }
    exit(0);
  }

  waitpid(pid1, NULL, 0);
  waitpid(pid2, NULL, 0);
  waitpid(pid3, NULL, 0);

  unlink(fifo1);
  unlink(fifo2);
  return 0;
}
