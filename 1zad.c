#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#define MAX_BUF 16
#define BGN 1 << 1
#define NO_BGN 1 << 2


int main(int argc, char const *argv[])
{
    
    if(argc < 1 || argc > 2) {
        printf("Error with arguments\n");
        return 0;
    }
    if(argc == 1) {    /*Процесс reader from fifo*/
        int fd, fd1;
        pid_t ppid, ppid1;
        int i, k, n;
        int count = 1;

        char * myfifo = "myfifo";
        char * fifoback = "fifoback";
        mkfifo(fifoback, 0666);
        char buf[1024 * 16];    
        fd = open(myfifo, O_RDONLY);
        fd1 = open(fifoback, O_WRONLY);

    step1:
        k = read(fd, &i, sizeof(int));
        if(k <= 0) {
            close(fd);
            close(fd1);
            return 0;
        }
        if(i != count) {
            write(fd1, &i, sizeof(int));
            read(fd, buf, sizeof(pid_t));
            write(fd1, buf, sizeof(pid_t));
            read(fd, &n, sizeof(int));
            write(fd1, &n, sizeof(int));
            read(fd, buf, n);
            write(fd1, buf, n);
            goto step1;
        }
        read(fd, &ppid, sizeof(pid_t));
        read(fd, &n, sizeof(int));
        read(fd, buf, n);
        write(STDOUT_FILENO, buf, n);
        count++;
    step2:
        k = read(fd, &i, sizeof(int));
        if(k <= 0) {
            close(fd);
            close(fd1);
            return 0;
        }
        if(i != count) {
            printf("\ncount i %d %d\n", count, i);
            write(fd1, &i, sizeof(int));
            read(fd, buf, sizeof(pid_t));
            write(fd1, buf, sizeof(pid_t));
            read(fd, &n, sizeof(int));
            write(fd1, &n, sizeof(int));
            read(fd, buf, n);
            write(fd1, buf, n);
            goto step2;
        }
        read(fd, &ppid1, sizeof(pid_t));
        if(ppid1 != ppid) {
            write(fd1, &count, sizeof(int));
            write(fd1, &ppid1, sizeof(pid_t));
            read(fd, &n, sizeof(int));
            write(fd1, &n, sizeof(int));
            read(fd, buf, n);
            write(fd1, buf, n);
            goto step2;
        }
        read(fd, &n, sizeof(int));
        read(fd, buf, n);
        write(STDOUT_FILENO, buf, n);
        count++;
        goto step2;


        write(STDOUT_FILENO, "\n", 1);
        close(fd1);
        close(fd);

        return 0;
    }
    if(argc == 2) {    /*Процесс writer in fifo*/
        int fd, fd1;
        int k, n;
        char buf[1024 * 16];
        int i = 1;
        char * myfifo = "myfifo";
        char * fifoback = "fifoback";
        pid_t pid;

        mkfifo(myfifo, 0666);

        FILE *f_in = NULL;
        f_in = fopen(argv[1], "r");

        fd = open(myfifo, O_WRONLY);
        fd1 = open(fifoback, O_RDONLY);

        pid = getpid();
        
        write(fd, &i, sizeof(int)); // begin
        write(fd, &pid, sizeof(pid_t));
        n = read(fileno(f_in), buf, MAX_BUF * sizeof(char)); 
        write(fd, &n, sizeof(int));
        write(fd, buf, n);
        while(n > 0 && k > 0) {
            i++;
            printf("\niiii %d\n", i);
            write(fd, &i, sizeof(int)); ///is not beginning
            write(fd, &pid, sizeof(pid_t));
            n = read(fileno(f_in), buf, MAX_BUF * sizeof(MAX_BUF));
            write(fd, &n, sizeof(int));
            write(fd, buf, n);
            //k = read(fd1, buf, sizeof(MAX_BUF));
            //write(fd, buf, n);
        }

        n = MAX_BUF;
        while(n > 0) {
            n = read(fd1, buf, sizeof(int) + sizeof(pid_t));
            write(fd, buf, n);
            read(fd1, &n, sizeof(int));
            write(fd, &n, sizeof(int));
            write(fd, buf, n);
        }

        close(fd1);
        close(fd);
        fclose(f_in);

        /* remove the FIFO */
        //unlink(myfifo);
        return 0;
    }

    return 0;
}
