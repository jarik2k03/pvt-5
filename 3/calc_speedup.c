#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (void) {

    FILE *serial = fopen("serial", "r");
    if (!serial) return -1;
    double serial_time;
    fscanf(serial, "%lf", &serial_time);

    for (size_t i = 1; i < 9; i++) {
        char filename[32] = "2x";
        char id[4];
        sprintf(id, "%ld", i);
        strncat(filename, id, 2);
        FILE *f = fopen(filename, "r+");
        double cur_time;
        fscanf(f, "%lf", &cur_time);
        fseek(f, 0, SEEK_SET);
        fprintf(f, "%lf", serial_time / cur_time);
        fclose(f);
    }
}
