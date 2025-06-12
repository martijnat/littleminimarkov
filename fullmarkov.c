#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>

#define DIM 128

#define next(h) (((h << 7))&((1<<28)-1))
/* #define next(h) ((((h&(3<<25))<<1)|((h&(7<<21))<<2)|((h&(3<<18))<<3)|(h&(7<<14))<<4|((h&(2047))<<7))&((1<<28)-1)) */

int main(int argc, char *argv[]) {
    if (argc != 2 && argc!=3) {
        fprintf(stderr, "Usage: %s model.bin [training_data.txt]\n", argv[0]);
        fprintf(stderr, "If no training data is provided, it will generate data instead\n", argv[0]);
        return 1;
    }

    if( argc==3 )
    {

    FILE *input = fopen(argv[2], "rb");
    if (!input) {
        perror("fopen input");
        return 1;
    }

    size_t size = (size_t)DIM * DIM * DIM * DIM;
    int16_t *model = calloc(size, sizeof(int16_t));
    if (!model) {
        perror("calloc");
        fclose(input);
        return 1;
    }

    uint8_t d;
    uint32_t h=0;
    while (fread(&d, 1, 1, input) == 1) {
        if (d >= 128) continue;
        h=next(h)|d;
        if (model[h] < INT16_MAX) model[h]++;
    }

    fclose(input);



        size_t packed_size = size / 4;
    uint8_t *quant_model = calloc(packed_size, 1);
    if (!quant_model) {
        perror("calloc");
        free(model);
        return 1;
    }

    for (size_t a = 0; a < DIM; a++) {
        for (size_t b = 0; b < DIM; b++) {
            for (size_t c = 0; c < DIM; c++) {
                size_t base = a*DIM*DIM*DIM + b*DIM*DIM + c*DIM;
                int16_t max_val = 0;
                for (size_t d = 0; d < DIM; d++) {
                    if (model[base + d] > max_val) max_val = model[base + d];
                }
                for (size_t d = 0; d < DIM; d++) {
                    uint8_t value;
                    if (max_val == 0) {
                        value = 0;
                    } else {
                        value = (3 * model[base + d]) / max_val;
                        if (value > 3) value = 3;
                    }
                    size_t index = base + d;
                    size_t byte_index = index >> 2;
                    int shift = (index & 3) * 2;
                    quant_model[byte_index] |= (value & 3) << shift;
                }
            }
        }
    }
    free(model);

    FILE *model_file = fopen(argv[1], "wb");
    if (!model_file) {
        perror("fopen model");
        free(model);
        return 1;
    }
    if (fwrite(quant_model, 1, packed_size, model_file) != size) {
        perror("fwrite");
    }

    fclose(model_file);
    free(quant_model);
    } else {
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
            uint32_t h=0;
            int space_repeat=0;
            while (space_repeat<7) {
                    size_t base = next(h);
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
                    h = next(h)|next_byte;
                    putchar(next_byte);
                    space_repeat++;
                    if(next_byte != 32)
                            space_repeat=0;
            }
            free(quant_model);
    }
    return 0;
}
