#include "./include/bmp.h"

uint8_t set_draw_color(bmp_obj *bmp, uint8_t R, uint8_t G, uint8_t B) {
    if ((bmp == NULL) || (bmp->brush_color == NULL)) {
        return 1;
    }

    memcpy(bmp->brush_color + 0, &B, 1);
    memcpy(bmp->brush_color + 1, &G, 1);
    memcpy(bmp->brush_color + 2, &R, 1);

    return 0;
}

void set_line_width(bmp_obj *bmp, uint8_t new_brush_size) {
    if (((new_brush_size & 1) == 0) || (bmp == NULL)) {
        return;
    }

    bmp->brush_size = new_brush_size;
}

void draw_dot(bmp_obj *bmp, int32_t pos_y1, int32_t pos_x1) {
    if ((bmp == NULL) || (bmp->img == NULL)) {
        return;
    }

    int32_t stop_i = 0, stop_j = 0;
    int32_t start_draw_bit = 0, half_brush_size = (int32_t)(bmp->brush_size / 2);

    if ((pos_x1 - half_brush_size) < 0) {
        stop_i = bmp->brush_size + half_brush_size - pos_x1;

        if ((pos_y1 - half_brush_size) < 0) {
            start_draw_bit = 0;
            stop_j = half_brush_size + pos_y1;
        } else if ((pos_y1 + half_brush_size) < bmp->infoheader.width) {
            start_draw_bit = (pos_y1 - half_brush_size) * BITS_PER_BYTE;
            stop_j = bmp->brush_size;
        } else {
            start_draw_bit = (pos_y1 - half_brush_size) * BITS_PER_BYTE;
            stop_j = bmp->infoheader.width - pos_y1 + half_brush_size;
        }
    } else if ((pos_x1 + half_brush_size) < bmp->infoheader.height) {
        stop_i = bmp->brush_size;

        if ((pos_y1 - half_brush_size) < 0) {
            start_draw_bit = (pos_x1 - half_brush_size) * bmp->infoheader.width * BITS_PER_BYTE;
            stop_j = half_brush_size + pos_y1;
        } else if ((pos_y1 + half_brush_size) < bmp->infoheader.width) {
            start_draw_bit = ((pos_x1 - half_brush_size) * bmp->infoheader.width + (pos_y1 - half_brush_size)) * BITS_PER_BYTE;
            stop_j = bmp->brush_size;
        } else {
            start_draw_bit = ((pos_x1 - half_brush_size) * bmp->infoheader.width + (pos_y1 - half_brush_size)) * BITS_PER_BYTE;
            stop_j = bmp->infoheader.width - pos_y1 + half_brush_size;
        }
    } else {
        stop_i = bmp->infoheader.height - pos_x1 + half_brush_size;

        if ((pos_y1 - half_brush_size) < 0) {
            start_draw_bit = (pos_x1 - half_brush_size) * bmp->infoheader.width * BITS_PER_BYTE;
            stop_j = half_brush_size + pos_y1;
        } else if ((pos_y1 + half_brush_size) < bmp->infoheader.width) {
            start_draw_bit = ((pos_x1 - half_brush_size) * bmp->infoheader.width + (pos_y1 - half_brush_size)) * BITS_PER_BYTE;
            stop_j = bmp->brush_size;
        } else {
            start_draw_bit = ((pos_x1 - half_brush_size) * bmp->infoheader.width + (pos_y1 - half_brush_size)) * BITS_PER_BYTE;
            stop_j = bmp->infoheader.width - pos_y1 + half_brush_size;
        }
    }

    for (int32_t iter_i = 0; iter_i < stop_i; ++iter_i) {
        for (int32_t iter_j = 0; iter_j < stop_j; ++iter_j) {
            memcpy(bmp->img + start_draw_bit, bmp->brush_color, BITS_PER_BYTE);
            start_draw_bit += BITS_PER_BYTE;
        }

        start_draw_bit += (bmp->infoheader.width - stop_j) * BITS_PER_BYTE;
    }
}

uint8_t draw_line(bmp_obj *bmp, int32_t pos_y1, int32_t pos_x1, int32_t pos_y2, int32_t pos_x2) {
    if ((bmp == NULL) || ((bmp->brush_size & 1) == 0) || (bmp->img == NULL)) {
        return 1;
    }

    if ((pos_y1 == pos_y2) && (pos_x1 == pos_x2)) {
        draw_dot(bmp, pos_y1, pos_x1);
        
        return 0;
    }

    if (pos_x1 == pos_x2) {
        if (pos_y1 > pos_y2) {
            int32_t temp = pos_y2;
            pos_y2 = pos_y1;
            pos_y1 = temp;
        }

        for (int32_t iter = pos_y1; iter <= pos_y2; ++iter) {
            draw_dot(bmp, iter, pos_x1);
        }

        return 0;
    }

    if (pos_y1 == pos_y2) {
        if (pos_x1 > pos_x2) {
            int32_t temp = pos_x2;
            pos_x2 = pos_x1;
            pos_x1 = temp;
        }

        for (int32_t iter = pos_x1; iter <= pos_x2; ++iter) {
            draw_dot(bmp, pos_y1, iter);
        }

        return 0;
    }

    int32_t delta_x = 0, delta_y = 0;

    if (abs(pos_x1 - pos_x2) <= abs(pos_y1 - pos_y2)) {
        if (pos_y1 > pos_y2) {
            int32_t temp_y = pos_y2, temp_x = pos_x2;
            pos_y2 = pos_y1;
            pos_x2 = pos_x1;
            pos_y1 = temp_y;
            pos_x1 = temp_x;
        }

        delta_x = pos_x2 - pos_x1;
        delta_y = pos_y2 - pos_y1;

        for (int32_t point_y = pos_y1; point_y <= pos_y2; ++point_y) {
            int32_t point_x = (delta_x * (point_y - pos_y1) + delta_y * pos_x1) / delta_y;

            draw_dot(bmp, point_y, point_x);
        }

        return 0;
    }

    if (pos_x1 > pos_x2) {
        int32_t temp_y = pos_y2, temp_x = pos_x2;
        pos_y2 = pos_y1;
        pos_x2 = pos_x1;
        pos_y1 = temp_y;
        pos_x1 = temp_x;
    }

    delta_x = pos_x2 - pos_x1;
    delta_y = pos_y2 - pos_y1;

    for (int32_t point_x = pos_x1; point_x <= pos_x2; ++point_x) {
        int32_t point_y = (delta_y * (point_x - pos_x1) + delta_x * pos_y1) / delta_x;

        draw_dot(bmp, point_y, point_x);
    }

    return 0;
}

uint8_t draw_rectangle(bmp_obj *bmp, int32_t pos_y1, int32_t pos_x1, int32_t rec_width, int32_t rec_height) {
    if (draw_line(bmp, pos_y1, pos_x1, pos_y1 + rec_width, pos_x1)) {
        return 1;
    }

    if (draw_line(bmp, pos_y1, pos_x1 + rec_height, pos_y1 + rec_width, pos_x1 + rec_height)) {
        return 1;
    }

    if (draw_line(bmp, pos_y1, pos_x1, pos_y1, pos_x1 + rec_height)) {
        return 1;
    }

    if (draw_line(bmp, pos_y1 + rec_width, pos_x1, pos_y1 + rec_width, pos_x1 + rec_height)) {
        return 1;
    }

    return 0;
}

uint8_t draw_triangle(bmp_obj *bmp, int32_t pos_y1, int32_t pos_x1, int32_t pos_y2, int32_t pos_x2, int32_t pos_y3, int32_t pos_x3) {
    if (draw_line(bmp, pos_y1, pos_x1, pos_y3, pos_x3)) {
        return 1;
    }

    if (draw_line(bmp, pos_y2, pos_x2, pos_y3, pos_x3)) {
        return 1;
    }

    if (draw_line(bmp, pos_y1, pos_x1, pos_y2, pos_x2)) {
        return 1;
    }

    return 0;
}
