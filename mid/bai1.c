#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define TRUE 1
#define FALSE 0

// funciton to check if file exists
int check_file_existence(char *file) {
    return access(file, F_OK) != -1;
}

// function to write data to file
int write_data(char *buf, size_t len) {
    int fileExist, fd;
    char *file = "output.dat";
    fileExist = check_file_existence(file);
    if (fileExist == FALSE) {
        fd = open(file, O_CREAT | O_WRONLY, S_IRUSR | S_IWUSR);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }
        write(fd, buf, len);
        close(fd);
    }
    return 0;
}

void main() {
    char *buf = "Hello, world!";
    size_t len = 13;
    write_data(buf, len);
}