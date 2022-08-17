#include "./include/bmp.h"

/**
 * @brief Function to overlap two or more images. The new image will be
 * drawn in front of the old image (always on top :) )
 * 
 * @param filename name of file to open the new image
 * @param bmp a pointer to an allocated bmp object
 * @param pos_y y coordonate of the left-down point
 * @param pos_x x coordonate of the left-down point
 * @return uint8_t 1 if insertion went successfully, 0 otherwise
 */
uint8_t insert_bmp(const char * const filename, bmp_obj *bmp, int32_t pos_y, int32_t pos_x) {
    /* Check if input data is valid */
    if ((filename == NULL) || (bmp == NULL) || (bmp->img == NULL)) {
        return 1;
    }

    FILE *fin = NULL;

    /* Open file for reading */
    if ((fin = fopen(filename, "rb")) == NULL) {
        return 1;
    }

    bmp_fileheader insert_file_header;
    bmp_infoheader insert_info_header;

    /* Read the file header */
    if (fread(&insert_file_header, sizeof(insert_file_header), 1, fin) != 1) {
        fclose(fin);
        
        return 1;
    }

    /* Check if the new image is a valid BMP */
    if (insert_file_header.file_mark1 != 'B' || insert_file_header.file_mark2 != 'M') {
        fclose(fin);
        
        return 1;
    }

    /* Read the information header */
    if (fread(&insert_info_header, sizeof(insert_info_header), 1, fin) != 1) {
        fclose(fin);
        
        return 1;
    }

    if (insert_info_header.bit_pix != COLOR_BITS) {
        fclose(fin);

        return 1;
    }

    /* Compute the size of the image itself */
    uint32_t insert_bit_size = insert_info_header.width * insert_info_header.height * BITS_PER_BYTE;
    uint8_t *insert_img = malloc(insert_bit_size);

    if (insert_img == NULL) {
        fclose(fin);

        return 1;
    }

    uint32_t padding = (4 - ((3 * insert_info_header.width) % 4)) % 4;
    int32_t insert_img_index = 0;
    int32_t col_dim = insert_info_header.width * BITS_PER_BYTE;

    /* Load the ew image into the program */
    for (int32_t iter_i = 0; iter_i < insert_info_header.height; ++iter_i) {
        for (int32_t iter_j = 0; iter_j < col_dim; ++iter_j) {
            if (fread(insert_img + insert_img_index, 1, 1, fin) != 1) {
                free(insert_img);
                insert_img = NULL;

                fclose(fin);
                
                return 1;
            }

            ++insert_img_index;
        }

        fseek(fin, padding, SEEK_CUR);
    }

    insert_img_index = bmp->infoheader.height * pos_y * BITS_PER_BYTE;
    int32_t img_index = bmp->infoheader.width * pos_x * BITS_PER_BYTE;

    /* Set start index for writing */
    if ((pos_y > 0) && (pos_x > 0)) {
        img_index = (pos_y + pos_x * bmp->infoheader.width) * BITS_PER_BYTE;
        insert_img_index = 0;
    }

    int stop_height = 0;
    int stop_width = 0;

    /* Set height limit */
    if ((insert_info_header.height + pos_x) > bmp->infoheader.height) {
        stop_height = bmp->infoheader.height - pos_x;
    } else {
        stop_height = insert_info_header.height;
    }

    /* Set width limit */
    if ((insert_info_header.width + pos_y) > bmp->infoheader.width) {
        stop_width = (bmp->infoheader.width - pos_y) * BITS_PER_BYTE;
    } else {
        stop_width = insert_info_header.width * BITS_PER_BYTE;
    }

    /* Overlap the new pixels over the old pixels */
    for (int32_t iter_i = 0; iter_i < stop_height; ++iter_i) {
        for (int32_t iter_j = 0; iter_j < stop_width; ++iter_j) {
            memcpy(bmp->img + img_index, insert_img + insert_img_index, 1);
            ++insert_img_index;
            ++img_index;
        }

        if ((pos_y + insert_info_header.width) >= bmp->infoheader.width) {
            img_index += (pos_y * BITS_PER_BYTE);
            insert_img_index += (insert_info_header.width - bmp->infoheader.width + pos_y) * BITS_PER_BYTE;
        } else {
            img_index += BITS_PER_BYTE * (bmp->infoheader.width - insert_info_header.width);
        }
    }

    free(insert_img);
    insert_img = NULL;

    fclose(fin);
    
    return 0;
}
