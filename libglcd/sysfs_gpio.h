/**
 * sysfs経由でGPIOを操作する
 */
int sysfs_gpio_open(const char *dir, unsigned pin);
int sysfs_gpio_close(int fd, unsigned pin);

int sysfs_gpio_set(int fd);
int sysfs_gpio_clr(int fd);
