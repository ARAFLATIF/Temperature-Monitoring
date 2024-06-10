#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

int main(int argc, char* argv[]) {
    int n = 0; // initialize number of samples
    int i; // loop control
    double min = 9999;
    double max = -9999;
    double sum = 0.0;
    double degs_c = 0.0;
    double degs_f = 0.0;
    int fd = 0;

    fd = open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY);
    assert(fd > 0); // if the device opened, fd will be positive

    i = 10;
    while (i) {
        char b[16] = {0};
        lseek(fd, 0, SEEK_SET);
        read(fd, b, sizeof(b));
        n++;
        degs_c = strtod(b, NULL) / 1000.0;
        degs_f = degs_c * (9.0 / 5.0) + 32.0;
        assert(degs_f > 0.0 && degs_f < 300.0);
        sum = sum + degs_f;
        if (degs_f < min) {
            min = degs_f;
        }
        if (degs_f > max) {
            max = degs_f;
        }
        sleep(1);
        i--;
    }

    printf("Read %d values\n", n);
    printf("Values in degrees F\n");
    assert(n);
    assert(min <= max);
    printf("\tMin %f Max %f Avg %f\n", min, max, sum / n);

    close(fd);
    return 0;
}
