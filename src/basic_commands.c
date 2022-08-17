#include "./include/bmp.h"

void edit_map(const char * const filename, bmp_obj *bmp) {
    if ((bmp == NULL) || (filename == NULL)) {
        return;
    }

    FILE *fin = NULL;

    if ((fin = fopen(filename, "rb")) == NULL) {
        return;
    }

    bmp_fileheader file_header;

    if (fread(&file_header, sizeof(file_header), 1, fin) != 1) {
        fclose(fin);
        
        return;
    }

    if (file_header.file_mark1 != 'B' || file_header.file_mark2 != 'M') {
        fclose(fin);
        
        return;
    }

    if (fread(&bmp->infoheader, sizeof(bmp->infoheader), 1, fin) != 1) {
        fclose(fin);
        
        return;
    }

    if (bmp->infoheader.bit_pix != COLOR_BITS) {
        fclose(fin);
        
        return;
    }

    size_t bit_size = bmp->infoheader.width * bmp->infoheader.height * BITS_PER_BYTE;

    bmp->img = malloc(bit_size);

    if (bmp->img == NULL) {
        fclose(fin);

        return;
    }

    int32_t padding = (4 - ((3 * bmp->infoheader.width) % 4)) % 4;
    int32_t img_index = 0;
    int32_t col_dim = bmp->infoheader.width * BITS_PER_BYTE;

    for (int32_t iter_i = 0; iter_i < bmp->infoheader.height; ++iter_i) {
        for (int32_t iter_j = 0; iter_j < col_dim; ++iter_j) {
            if (fread(bmp->img + img_index, 1, 1, fin) != 1) {
                fclose(fin);
                
                free(bmp->img);
                
                return;
            }

            ++img_index;
        }

        fseek(fin, padding, SEEK_CUR);
    }

    fclose(fin);
}

uint8_t save_map(const char * const filename, bmp_obj *bmp) {
    if ((filename == NULL) || (bmp == NULL) || (bmp->img == NULL)) {
        printf("-1-\n");
        return 1;
    }

    FILE *fout = NULL;

    if ((fout = fopen(filename, "wb")) == NULL) {
        printf("-2-\n");
        return 1;
    }

    bmp_fileheader file_header;

    file_header.file_mark1 = 'B';
    file_header.file_mark2 = 'M';
    file_header.bf_size = BMP_FILE_SIZE + bmp->infoheader.width * bmp->infoheader.height * BITS_PER_BYTE;
    file_header.unused1 = 0;
    file_header.unused2 = 0;
    file_header.img_data_offset = BMP_FILE_SIZE;

    if (fwrite(&file_header, sizeof(file_header), 1, fout) != 1) {
        fclose(fout);

        free(bmp->img);
        bmp->img = NULL;

        printf("-3-\n");
        return 1;
    }

    if (fwrite(&bmp->infoheader, sizeof(bmp->infoheader), 1, fout) != 1) {
        fclose(fout);

        free(bmp->img);
        bmp->img = NULL;

        printf("-4-\n");
        return 1;
    }

    uint32_t padding = (4 - ((3 * bmp->infoheader.width) % 4)) % 4;
    uint8_t pad_bit = 0;
    int32_t img_index = 0;
    int32_t col_dim = bmp->infoheader.width * BITS_PER_BYTE;

    for (int32_t iter_i = 0; iter_i < bmp->infoheader.height; ++iter_i) {
        for (int32_t iter_j = 0; iter_j < col_dim; ++iter_j) {
            if (fwrite(bmp->img + img_index, 1, 1, fout) != 1) {
                fclose(fout);

                free(bmp->img);
                bmp->img = NULL;

                printf("-5-\n");
                return 1;
            }

            ++img_index;
        }

        if (fwrite(&pad_bit, 1, padding, fout) < padding) {
            fclose(fout);

            free(bmp->img);
            bmp->img = NULL;

            printf("-6-\n");
            return 1;
        }
    }

    fclose(fout);

    return 0;
}

void quit_map(bmp_obj *bmp) {
    if (bmp != NULL) {
        if (bmp->img != NULL) {
            free(bmp->img);
            bmp->img = NULL;
        }

        if (bmp->brush_color != NULL) {
            free(bmp->brush_color);
            bmp->brush_color = NULL;
        }
    }
}