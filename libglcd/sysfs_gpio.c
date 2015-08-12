/**
 * sysfs経由でGPIOを操作する
 */
#include <fcntl.h>
#include <string.h>
#include <stdio.h>

int sysfs_gpio_open(const char *dir, unsigned pin)
{
    int fd, len;
    char buf[32];

    /* 使用を宣言。writeの返り値を調べるのが本来だが、
     * 継続使用する場合もあるのでチェックしない */
    if((fd = open("/sys/class/gpio/export", O_WRONLY)) < 0)
	goto failed;
    sprintf(buf, "%d", pin);
    len = strlen(buf);
    write(fd, buf, len);
    close(fd);

    /* 入出力を設定 */
    sprintf(buf, "/sys/class/gpio/gpio%d/direction", pin);
    if((fd = open(buf, O_WRONLY)) < 0)
	goto failed;
    len = strlen(dir);
    if(write(fd, dir, len) < len)
	goto failed;
    close(fd);

    /* GPIOファイルをオープン */
    sprintf(buf, "/sys/class/gpio/gpio%d/value", pin);
    if((fd = open(buf, O_WRONLY)) < 0)
	goto failed;
    return fd;

  failed:
    if(fd >= 0)
	close(fd);
    if((fd = open("/sys/class/gpio/unexport", O_WRONLY)) < 0)
	return -1;

    sprintf(buf, "%d", pin);
    write(fd, buf, strlen(buf));
    close(fd);
    return -1;
}

int sysfs_gpio_close(int fd, unsigned pin)
{
    char buf[32];

    /* GPIOファイルをクローズ */
    close(fd);

    /* 使用完了を宣言 */
    if((fd = open("/sys/class/gpio/unexport", O_WRONLY)) < 0)
	return -1;
    sprintf(buf, "%d", pin);
    write(fd, buf, strlen(buf));
    close(fd);
}

int sysfs_gpio_set(int fd)
{
    return write(fd, "1", 1) != 1;
}

int sysfs_gpio_clr(int fd)
{
    return write(fd, "0", 1) != 1;
}
