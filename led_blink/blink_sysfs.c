#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#define PORT_NR 2

static volatile int interrupted = 0;

static void handler(int sig)
{
    interrupted = 1;
}

int main(int argc, char *argv[])
{
    int fd;
    char buf[256];

    chdir("/sys/class/gpio");

    /* enable GPIO port */
    sprintf(buf, "%d", PORT_NR);
    fd = open("export", O_WRONLY);
    write(fd, buf, strlen(buf));
    close(fd);

    signal(SIGINT, handler);

    /* setup GPIO */
    sprintf(buf, "gpio%d/direction", PORT_NR);
    fd = open(buf, O_WRONLY);
    strcpy(buf, "out");
    write(fd, buf, strlen(buf));
    close(fd);

    /* loop */
    sprintf(buf, "gpio%d/value", PORT_NR);
    fd = open(buf, O_WRONLY);
    while(!interrupted) {
	write(fd, "1", 1);
	write(fd, "0", 1);
    }
    close(fd);

    /* disable GPIO */
    sprintf(buf, "%d", PORT_NR);
    fd = open("unexport", O_WRONLY);
    write(fd, buf, strlen(buf));
    close(fd);

    return 0;
}
