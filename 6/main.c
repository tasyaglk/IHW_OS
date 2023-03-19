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
  pid_t pid;

  char *in_filename = argv[1];
  char *out_filename = argv[2];
  char buf[BUF_SIZE + 1];
  char ans[BUF_SIZE + 1];
  int pfd[2];
  if (pipe(pfd) == -1) {
    printf("Error: failed to create pipe pfd.\n");
    return 1;
  }

  pid = fork();

  if (pid == -1) {
    printf("Error forking process 1\n");
    return 1;
  } else if (pid == 0) {
    close(pfd[0]);

    int in_fd = open(in_filename, O_RDONLY);
    if (in_fd == -1) {
      printf("Error: failed to open input file.\n");
      exit(-1);
    }

    ssize_t read_bytes;
    while ((read_bytes = read(pfd[0], buf, BUF_SIZE)) > 0) {
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
      if (write(pfd[1], ans, j) == -1) {
        printf("Error: failed to write to pipe 2");
        exit(1);
      }
    }
      if (close(pfd[1]) < 0) {
        printf("Error: failed to close");
        exit(1);
      }
    if (close(in_fd) < 0) {
      printf("Error: failed to close fail");
      exit(1);
    }
    exit(0);
  } else {
    int status;
    waitpid(pid, &status, 0);

    close(pfd[1]);
    ssize_t read_bytes = read(pfd[0], ans, sizeof(ans));
    int out_fd = open(out_filename, O_WRONLY | O_CREAT, 0666);
    if (out_fd == -1) {
      printf("Error: failed to open output file\n");
      exit(-1);
    }

    if (write(out_fd, ans, read_bytes) == -1) {
      perror("Failed to write to file");
      exit(1);
    }
      if (close(pfd[0]) < 0) {
        printf("Error: failed to close");
        exit(1);
      }
    if (close(out_fd) < 0) {
      printf("Error: failed to close fail");
      exit(1);
    }
    exit(0);
  }
  return 0;
}
