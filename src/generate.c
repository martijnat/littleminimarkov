#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define DIM 128
#define OUTPUT_SIZE 12345

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s quant.bin\n", argv[0]);
        return 1;
    }

    FILE *quant_file = fopen(argv[1], "rb");
    if (!quant_file) {
        perror("fopen quant");
        return 1;
    }

    size_t packed_size = (size_t)DIM * DIM * DIM * DIM / 4;
    uint8_t *quant_model = malloc(packed_size);
    if (!quant_model) {
        perror("malloc");
        fclose(quant_file);
        return 1;
    }

    if (fread(quant_model, 1, packed_size, quant_file) != packed_size) {
        perror("fread");
        fclose(quant_file);
        free(quant_model);
        return 1;
    }
    fclose(quant_file);

    srand(time(NULL));
    uint8_t a = 0, b = 0, c = 0;
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        size_t base = (size_t)a*DIM*DIM*DIM + b*DIM*DIM + c*DIM;
        uint8_t weights[128];
        uint32_t total_weight = 0;

        for (int d = 0; d < 128; d++) {
            size_t index = base + d;
            size_t byte_index = index >> 2;
            int shift = (index & 3) * 2;
            uint8_t weight = (quant_model[byte_index] >> shift) & 3;
            weights[d] = weight;
            total_weight += weight;
        }

        uint8_t next_byte;
        if (total_weight == 0) {
                next_byte = rand() % 128;
        } else {
            uint32_t r = rand() % total_weight;
            uint32_t sum = 0;
            for (int d = 0; d < 128; d++) {
                sum += weights[d];
                if (sum > r) {
                    next_byte = d;
                    break;
                }
            }
        }

        putchar(next_byte);
        a = b;
        b = c;
        c = next_byte;
    }

    free(quant_model);
    return 0;
}
