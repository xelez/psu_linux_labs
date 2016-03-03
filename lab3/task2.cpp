//Author: Ankudinov Alexander

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#ifdef DEBUG
#define DBG(fmt, a...) fprintf(stderr, "In %s on line %d: " fmt "\n", __FUNCTION__, __LINE__, ## a);
#else
#define DBG(n, a...)
#endif

const int MAX_N = 1000;

struct mymsgbuf {
    long mtype;
    int n;
    int a[MAX_N];
};

int queue_id = -1;
int n;
int res[MAX_N];
mymsgbuf buf;

void worker() {
    DBG("I am worker");
    mymsgbuf vec;
    
    //receiving n and vector (mtype == 1)
    msgrcv(queue_id, &vec, sizeof(int)*(MAX_N+1), 1, 0);
    n = vec.n;

    DBG("I received n = %d", n);

    while (1) {
        //receiving row (mtype == 2)
        int res = msgrcv(queue_id, &buf, sizeof(int)*(MAX_N+1), 2, 0);
        if (res == -1)
            exit(1);
        
        DBG("I received row i = %d", buf.n);
        int sum = 0;
        for (int i = 0; i < n; ++i)
            sum += vec.a[i] * buf.a[i];
        buf.a[0] = sum;

        // sending row
        buf.mtype = 3;
        msgsnd(queue_id, &buf, sizeof(int)*2, 0);
    }
}

void handle_error(const char * msg) {
    perror(msg);
    if (queue_id != -1)
        msgctl(queue_id, IPC_RMID, 0);
    exit(1);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s k matrix_file\n", argv[0]);
        exit(1);
    }

    int k = atoi(argv[1]);
    if (k < 1) {
        printf("Number of processes must be a number and greater than zero.\n");
        exit(1);
    }

    // creating queue
    queue_id = msgget(IPC_PRIVATE, 0660);
    if (queue_id == -1)
        handle_error("Can't create queue: ");

    for (int i = 0; i < k; ++i) {
        pid_t pid = fork();
        if (pid == 0)
            worker();
        if (pid == -1)
            handle_error("Can't fork: ");
    }

    FILE * f = fopen(argv[2], "r");
    if (f == NULL)
        handle_error("Can't open input file");

    fscanf(f, "%d", &n);
    for (int i = 0; i < n; ++i)
        fscanf(f, "%d", &buf.a[i]);


    buf.mtype = 1;
    buf.n = n;
    for (int i = 0; i < k; ++i)
        msgsnd(queue_id, &buf, sizeof(int)*(n+1), 0);

    buf.mtype = 2;
    for (int i = 0; i < n; ++i) {
        buf.n = i;

        for (int j = 0; j < n; ++j)
            fscanf(f, "%d", &buf.a[j]);

        msgsnd(queue_id, &buf, sizeof(int)*(n+1), 0);
    }

    for (int i = 0; i < n; ++i) {
        msgrcv(queue_id, &buf, sizeof(int)*(MAX_N+1), 3, 0);
        res[buf.n] = buf.a[0];
    }

    // deleting queue!!
    msgctl(queue_id, IPC_RMID, 0);

    for (int i = 0; i < n; ++i)
        printf(" %d\n", res[i]);

    return 0;
}


