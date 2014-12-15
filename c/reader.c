#include <libhackrf/hackrf.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

const long BUFFER_SIZE = 1e6;
uint8_t buffer[BUFFER_SIZE];
long buffer_pos = 0;

#define CHECK_STATUS(status, message) \
    if (status != 0) { \
        printf("FAIL: %s\n", message); \
        hackrf_close(device); \
        hackrf_exit(); \
        exit(-1); \
    } \

int receive_sample_block(hackrf_transfer *transfer) {
    printf("block length: %d\n", transfer->valid_length);
    for (int i = 0; i < transfer->valid_length; i+= 2) {
        if (buffer_pos < BUFFER_SIZE) {
            buffer[buffer_pos++] = transfer->buffer[i];
        }
    }
    if (buffer_pos >= BUFFER_SIZE) {
        FILE *fp = fopen("out.raw", "wb");
        if (fp) {
            fwrite(buffer, BUFFER_SIZE, 1, fp);
            fclose(fp);
            printf("Written file.\n");
            exit(0);
        }
    }
    return 0;
}

int main(int argc, char **argv) {
    
    int status;
    hackrf_device *device;

    status = hackrf_init();
    CHECK_STATUS(status, "hackrf_init");

    status = hackrf_open(&device);
    CHECK_STATUS(status, "hackrf_open");

    status = hackrf_set_freq(device, 100.9e6);
    CHECK_STATUS(status, "hackrf_set_freq");

    status = hackrf_set_sample_rate(device, 10e6);
    CHECK_STATUS(status, "hackrf_set_sample_rate");

    status = hackrf_start_rx(device, receive_sample_block, NULL);
    CHECK_STATUS(status, "hackrf_start_rx");

    sleep(1);

    hackrf_stop_rx(device);
    hackrf_close(device);
    hackrf_exit();
    return 0;
}