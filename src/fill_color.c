#include "./include/bmp.h"

void fill_pixel(bmp_obj *bmp, uint8_t *old_brush_color, int32_t pos_y, int32_t pos_x) {
    if ((pos_y < 0 || pos_y >= bmp->infoheader.width) || (pos_x < 0 || pos_x >= bmp->infoheader.height)) {
        return;
    }

    int32_t point = (pos_y + pos_x * bmp->infoheader.width) * BITS_PER_BYTE;

    if ((bmp->img[point] != old_brush_color[0]) || (bmp->img[point + 1] != old_brush_color[1]) || (bmp->img[point + 2] != old_brush_color[2])) {
        return;
    }

    if ((bmp->img[point] == bmp->brush_color[0]) && (bmp->img[point + 1] == bmp->brush_color[1]) && (bmp->img[point + 2] == bmp->brush_color[2])) {
        return;
    }

    memcpy(bmp->img + point, bmp->brush_color, BITS_PER_BYTE);

    fill_pixel(bmp, old_brush_color, pos_y - 1, pos_x);
    fill_pixel(bmp, old_brush_color, pos_y + 1, pos_x);
    fill_pixel(bmp, old_brush_color, pos_y, pos_x + 1);
    fill_pixel(bmp, old_brush_color, pos_y, pos_x - 1);
}

void fill_map(bmp_obj *bmp, int32_t pos_y, int32_t pos_x) {
    if ((bmp == NULL) || (bmp->img == NULL)) {
        return;
    }

    uint8_t *old_brush_color = malloc(BITS_PER_BYTE);

    if (old_brush_color == NULL) {
        return;
    }

    int32_t point = (pos_y + pos_x * bmp->infoheader.width) * BITS_PER_BYTE;

    memcpy(old_brush_color, bmp->img + point, BITS_PER_BYTE);

    if ((old_brush_color[0] == bmp->brush_color[0]) && (old_brush_color[1] == bmp->brush_color[1]) && (old_brush_color[2] == bmp->brush_color[2])) {
        free(old_brush_color);
        return;
    }

    fill_pixel(bmp, old_brush_color, pos_y, pos_x);

    free(old_brush_color);
    old_brush_color = NULL;
}
