#include "./include/bmp.h"

/**
 * @brief Function to load the bmp image into the program
 * 
 * @param filename name of the bmp file to open
 * @param bmp a pointer to an allocated bmp object to store the bmp data
 */
void edit_map(const char * const filename, bmp_obj *bmp) {
    /* Check if the input data is valid */
    if ((bmp == NULL) || (filename == NULL)) {
        return;
    }

    FILE *fin = NULL;

    /* Open file to read */
    if ((fin = fopen(filename, "rb")) == NULL) {
        return;
    }

    bmp_fileheader file_header;

    /* Read the header file of the bmp image */
    if (fread(&file_header, sizeof(file_header), 1, fin) != 1) {
        fclose(fin);
        
        return;
    }

    /* Check if the file is a valid bmp image */
    if (file_header.file_mark1 != 'B' || file_header.file_mark2 != 'M') {
        fclose(fin);
        
        return;
    }

    /* Read the information header */
    if (fread(&bmp->infoheader, sizeof(bmp->infoheader), 1, fin) != 1) {
        fclose(fin);
        
        return;
    }

    /* Check if color pixel bits are valid */
    if (bmp->infoheader.bit_pix != COLOR_BITS) {
        fclose(fin);
        
        return;
    }

    /* Check the size of the image itself */
    size_t bit_size = bmp->infoheader.width * bmp->infoheader.height * BITS_PER_BYTE;

    bmp->img = malloc(bit_size);

    if (bmp->img == NULL) {
        fclose(fin);

        return;
    }

    /* Compute the padding */
    int32_t padding = (4 - ((3 * bmp->infoheader.width) % 4)) % 4;
    int32_t img_index = 0;
    int32_t col_dim = bmp->infoheader.width * BITS_PER_BYTE;

    /* Load the image into the program */
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

/**
 * @brief Function to save a bmp file into a file after it
 * was loaded and modified by the program.
 * 
 * @param filename name of the bmp file to open
 * @param bmp a pointer to an allocated bmp object to save the bmp data 
 * @return uint8_t 1 if saving went ok, 0 otherwise
 */
uint8_t save_map(const char * const filename, bmp_obj *bmp) {
    /* Check if input data is valid */
    if ((filename == NULL) || (bmp == NULL) || (bmp->img == NULL)) {
        return 1;
    }

    FILE *fout = NULL;

    /* Open file for writing */
    if ((fout = fopen(filename, "wb")) == NULL) {
        return 1;
    }

    bmp_fileheader file_header;

    /* Set the header file */
    file_header.file_mark1 = 'B';
    file_header.file_mark2 = 'M';
    file_header.bf_size = BMP_FILE_SIZE + bmp->infoheader.width * bmp->infoheader.height * BITS_PER_BYTE;
    file_header.unused1 = 0;
    file_header.unused2 = 0;
    file_header.img_data_offset = BMP_FILE_SIZE;

    /* Write the header file */
    if (fwrite(&file_header, sizeof(file_header), 1, fout) != 1) {
        fclose(fout);

        free(bmp->img);
        bmp->img = NULL;

        return 1;
    }

    /* Read the information headeder data */
    if (fwrite(&bmp->infoheader, sizeof(bmp->infoheader), 1, fout) != 1) {
        fclose(fout);

        free(bmp->img);
        bmp->img = NULL;

        return 1;
    }

    /* Compute the padding to skip */
    uint32_t padding = (4 - ((3 * bmp->infoheader.width) % 4)) % 4;
    uint8_t pad_bit = 0;
    int32_t img_index = 0;
    int32_t col_dim = bmp->infoheader.width * BITS_PER_BYTE;

    /* Write the image into the bmp data file */
    for (int32_t iter_i = 0; iter_i < bmp->infoheader.height; ++iter_i) {
        for (int32_t iter_j = 0; iter_j < col_dim; ++iter_j) {
            if (fwrite(bmp->img + img_index, 1, 1, fout) != 1) {
                fclose(fout);

                free(bmp->img);
                bmp->img = NULL;

                return 1;
            }

            ++img_index;
        }

        if (fwrite(&pad_bit, 1, padding, fout) < padding) {
            fclose(fout);

            free(bmp->img);
            bmp->img = NULL;

            return 1;
        }
    }

    fclose(fout);

    return 0;
}

/**
 * @brief Function to free every memory loaded into the program
 * 
 * @param bmp a pointer to an allocated bmp object
 */
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