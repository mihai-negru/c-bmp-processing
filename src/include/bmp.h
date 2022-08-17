#ifndef BMP_H_
#define BMP_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include <errno.h>
#include "./bmp_header.h"

#define INSTR_MAX_LEN 100
#define BRUSH_MAX_LEN 3
#define BITS_PER_BYTE 3
#define BMP_FILE_SIZE 54
#define COLOR_BITS 24

typedef struct {
    bmp_infoheader  infoheader;
    uint8_t         *img;
    uint8_t         brush_size;
    uint8_t         *brush_color;
} bmp_obj;

void                    edit_map                (const char * const filename, bmp_obj *bmp);
uint8_t                 save_map                (const char * const filename, bmp_obj *bmp);
void                    quit_map                (bmp_obj *bmp);

uint8_t                 insert_bmp              (const char * const filename, bmp_obj *bmp, int32_t pos_y, int32_t pos_x);

uint8_t                 set_draw_color          (bmp_obj *bmp, uint8_t R, uint8_t G, uint8_t B);
void                    set_line_width          (bmp_obj *bmp, uint8_t new_brush_size);
void                    draw_dot                (bmp_obj *bmp, int32_t pos_y1, int32_t pos_x1);
uint8_t                 draw_line               (bmp_obj *bmp, int32_t pos_y1, int32_t pos_x1, int32_t pos_y2, int32_t pos_x2);
uint8_t                 draw_rectangle          (bmp_obj *bmp, int32_t pos_y1, int32_t pos_x1, int32_t rec_width, int32_t rec_height);
uint8_t                 draw_triangle           (bmp_obj *bmp, int32_t pos_y1, int32_t pos_x1, int32_t pos_y2, int32_t pos_x2, int32_t pos_y3, int32_t pos_x3);

void                    fill_map                (bmp_obj *bmp, int32_t pos_y, int32_t pos_x);

#endif /* BMP_H_ */
