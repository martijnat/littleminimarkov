#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define DIM 128

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s input.txt model.bin\n", argv[0]);
        return 1;
    }

    FILE *input = fopen(argv[1], "rb");
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

    uint8_t a = 0, b = 0, c = 0, d;
    while (fread(&d, 1, 1, input) == 1) {
        if (d >= 128) continue;
        size_t index = (size_t)a * DIM*DIM*DIM + b * DIM*DIM + c * DIM + d;
        if (index < size) {
            if (model[index] < INT16_MAX) model[index]++;
        }
        a = b;
        b = c;
        c = d;
    }

    fclose(input);

    FILE *model_file = fopen(argv[2], "wb");
    if (!model_file) {
        perror("fopen model");
        free(model);
        return 1;
    }

    if (fwrite(model, sizeof(int16_t), size, model_file) != size) {
        perror("fwrite");
    }

    fclose(model_file);
    free(model);
    return 0;
}
