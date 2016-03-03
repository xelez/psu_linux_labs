//Author: Ankudinov Alexander

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cmath>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string>


#ifdef DEBUG
#define DBG(fmt, a...) fprintf(stderr, "In %s on line %d: " fmt "\n", __FUNCTION__, __LINE__, ## a);
#else
#define DBG(n, a...) 
#endif


int read_fd;
FILE *write_file;

char username[100];

void open_pipes(char *fname) {
    read_fd = open(fname, O_NONBLOCK | O_RDONLY);
    if (read_fd == -1) {
        perror("Error opening pipe for reading: ");
        exit(1);
    }

    write_file = fopen(fname, "w");
    if (!write_file) {
        perror("couldn't open pipe for writing\n");
        exit(1);
    }
}

const int BUF_SIZE = 256;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s fifo_file\n", argv[0]);
        exit(1);
    }

    open_pipes(argv[1]);

    fd_set read_fds;
    char buf[BUF_SIZE];
    
    DBG("tratata");

    while (1) {
        FD_ZERO(&read_fds);
        FD_SET(0, &read_fds);
        FD_SET(read_fd, &read_fds);
        DBG("d");
        
        int ret = select(read_fd+1, &read_fds, NULL, NULL, NULL);
        DBG("ret = %d", ret);
        if (ret == -1) {
            perror("select(): ");
            exit(1);
        }
        DBG("d");
        
        if (FD_ISSET(read_fd, &read_fds)) {
            int s = read(read_fd, buf, BUF_SIZE-1);
            DBG("s = %d", s);
            if (s > 0) {
                buf[s] = 0;
                printf("%s", buf);
            }
        }
        DBG("d");

        if (FD_ISSET(0, &read_fds)) {
            int s = read(0, buf, BUF_SIZE-1);
            DBG("s = %d", s);
            if (s > 0) {
                buf[s] = 0;
                fprintf(write_file, "%s", buf);
                fflush(write_file);
            }
            usleep(100);
        }

        DBG("d");
    }
    return 0;
}


