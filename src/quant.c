#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DIM 128

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s model.bin quant.bin\n", argv[0]);
        return 1;
    }

    FILE *model_file = fopen(argv[1], "rb");
    if (!model_file) {
        perror("fopen model");
        return 1;
    }

    size_t size = (size_t)DIM * DIM * DIM * DIM;
    int16_t *model = malloc(size * sizeof(int16_t));
    if (!model) {
        perror("malloc");
        fclose(model_file);
        return 1;
    }

    if (fread(model, sizeof(int16_t), size, model_file) != size) {
        perror("fread");
        fclose(model_file);
        free(model);
        return 1;
    }
    fclose(model_file);

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

    FILE *quant_file = fopen(argv[2], "wb");
    if (!quant_file) {
        perror("fopen quant");
        free(quant_model);
        return 1;
    }

    if (fwrite(quant_model, 1, packed_size, quant_file) != packed_size) {
        perror("fwrite");
    }

    fclose(quant_file);
    free(quant_model);
    return 0;
}
