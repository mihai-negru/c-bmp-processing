#include "./include/bmp.h"

/**
 * @brief Subroutine function to fill all pixels from a region with the same brush color.
 * 
 * @param bmp a pointer to an allocated bmp object
 * @param old_brush_color old brush color
 * @param pos_y y coordonate of the current drawing pixel
 * @param pos_x x coordonate of the current drawing pixel
 */
void fill_pixel(bmp_obj *bmp, uint8_t *old_brush_color, int32_t pos_y, int32_t pos_x) {
    /* Check if the coordinated are valid */
    if ((pos_y < 0 || pos_y >= bmp->infoheader.width) || (pos_x < 0 || pos_x >= bmp->infoheader.height)) {
        return;
    }

    int32_t point = (pos_y + pos_x * bmp->infoheader.width) * BITS_PER_BYTE;

    /* Check if recolouring is needed */
    if ((bmp->img[point] != old_brush_color[0]) || (bmp->img[point + 1] != old_brush_color[1]) || (bmp->img[point + 2] != old_brush_color[2])) {
        return;
    }

    /* Check if the recoluring is needed */
    if ((bmp->img[point] == bmp->brush_color[0]) && (bmp->img[point + 1] == bmp->brush_color[1]) && (bmp->img[point + 2] == bmp->brush_color[2])) {
        return;
    }

    /* Recolor the pixel */
    memcpy(bmp->img + point, bmp->brush_color, BITS_PER_BYTE);

    /* Recursively recolor the neighbours pixels if needed */
    fill_pixel(bmp, old_brush_color, pos_y - 1, pos_x);
    fill_pixel(bmp, old_brush_color, pos_y + 1, pos_x);
    fill_pixel(bmp, old_brush_color, pos_y, pos_x + 1);
    fill_pixel(bmp, old_brush_color, pos_y, pos_x - 1);
}

/**
 * @brief Function to fill with color a region
 * 
 * @param bmp a pointer to an allocated bmp object
 * @param pos_y y coordonate to start filling with color
 * @param pos_x x coordonate to start filling with color
 */
void fill_map(bmp_obj *bmp, int32_t pos_y, int32_t pos_x) {
    /* Check if input data is valid */
    if ((bmp == NULL) || (bmp->img == NULL)) {
        return;
    }

    /* Allocate the old brush color */
    uint8_t *old_brush_color = malloc(BITS_PER_BYTE);

    if (old_brush_color == NULL) {
        return;
    }

    int32_t point = (pos_y + pos_x * bmp->infoheader.width) * BITS_PER_BYTE;

    /* Compute the old brush color */
    memcpy(old_brush_color, bmp->img + point, BITS_PER_BYTE);

    /* Check if old brush color is the same as the new one */
    if ((old_brush_color[0] == bmp->brush_color[0]) && (old_brush_color[1] == bmp->brush_color[1]) && (old_brush_color[2] == bmp->brush_color[2])) {
        free(old_brush_color);
        return;
    }

    /* Fill the region */
    fill_pixel(bmp, old_brush_color, pos_y, pos_x);

    free(old_brush_color);
    old_brush_color = NULL;
}
