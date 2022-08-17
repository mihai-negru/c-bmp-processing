#include "./include/bmp.h"

int main(void) {
    int quit_var = 1;

    char *instr_str = malloc(INSTR_MAX_LEN);

    if (instr_str == NULL) {
        return EXIT_FAILURE;
    }

    bmp_obj *bmp = malloc(sizeof(*bmp));

    if (bmp == NULL) {
        free(instr_str);
        instr_str = NULL;

        return EXIT_FAILURE;
    }

    bmp->brush_color = malloc(BRUSH_MAX_LEN);
    bmp->brush_size = 1;
    
    if (bmp->brush_color == NULL) {
        free(instr_str);
        instr_str = NULL;

        return EXIT_FAILURE;
    }

    while (quit_var == 1) {
        if (scanf("%s", instr_str) > 1) {
            return EXIT_FAILURE;
        }

        if (instr_str[0] == 's') {
            if (instr_str[1] == 'a') {
                if (scanf("%s", instr_str) > 1) {
                    return EXIT_FAILURE;
                }

                if (save_map(instr_str, bmp) != 0) {
                    printf("Error to save!!!\n");
                }
            } else if (instr_str[1] == 'e') {
                if (scanf("%s", instr_str) > 1) {
                    return EXIT_FAILURE;
                }

                if (instr_str[0] == 'd') {
                    uint8_t red = 0, green = 0, blue = 0;

                    if (scanf("%hhu%hhu%hhu", &red, &green, &blue) > 3) {
                        return EXIT_FAILURE;
                    }

                    if (set_draw_color(bmp, red, green, blue) != 0) {
                        printf("Error to set brush color!!!\n");
                    }
                } else if (instr_str[0] == 'l') {
                    uint8_t line_width = 1;
                    
                    if (scanf("%hhu", &line_width) > 1) {
                        return EXIT_FAILURE;
                    }

                    set_line_width(bmp, line_width);
                } else {
                    free(instr_str);
                    instr_str = NULL;

                    return EXIT_FAILURE;
                }
            } else {
                free(instr_str);
                instr_str = NULL;
                    
                return EXIT_FAILURE;
            }
        } else if (instr_str[0] == 'e') {
            if (scanf("%s", instr_str) > 1) {
                return EXIT_FAILURE;
            }

            edit_map(instr_str, bmp);

            if (bmp->img == NULL) {
                return EXIT_FAILURE;
            }
        } else if (instr_str[0] == 'i') {
            if (scanf("%s", instr_str) > 1) {
                return EXIT_FAILURE;
            }

            uint32_t pos_x = 0, pos_y = 0;

            if (scanf("%u%u", &pos_y, &pos_x) > 2) {
                return EXIT_FAILURE;
            }

            if (insert_bmp(instr_str, bmp, pos_y, pos_x) != 0) {
                printf("Error to insert image!\n");
            }
        } else if (instr_str[0] == 'd') {
            if (scanf("%s", instr_str) > 1) {
                return EXIT_FAILURE;
            }

            if (instr_str[0] == 'l') {
                int32_t pos_y1 = 0, pos_x1 = 0, pos_y2 = 0, pos_x2 = 0;

                if (scanf("%d%d%d%d", &pos_y1, &pos_x1, &pos_y2, &pos_x2) > 4) {
                    return EXIT_FAILURE;
                }

                if (draw_line(bmp, pos_y1, pos_x1, pos_y2, pos_x2) != 0) {
                    printf("Error to draw a line!!!\n");
                }
            } else if (instr_str[0] == 'r') {
                int32_t pos_y1 = 0, pos_x1 = 0, width_rec = 0, height_rec = 0;
                
                if (scanf("%d%d%d%d", &pos_y1, &pos_x1, &width_rec, &height_rec) > 4) {
                    return EXIT_FAILURE;
                }

                if (draw_rectangle(bmp, pos_y1, pos_x1, width_rec, height_rec) != 0) {
                    printf("Error to draw a rectangle!!!\n");
                }
            } else if (instr_str[0] == 't') {
                int32_t pos_y1 = 0, pos_x1 = 0;
                int32_t pos_y2 = 0, pos_x2 = 0;
                int32_t pos_y3 = 0, pos_x3 = 0;
                
                if (scanf("%d%d%d%d%d%d", &pos_y1, &pos_x1, &pos_y2, &pos_x2, &pos_y3, &pos_x3) > 6) {
                    return EXIT_FAILURE;
                }

                if (draw_triangle(bmp, pos_y1, pos_x1, pos_y2, pos_x2, pos_y3, pos_x3)) {
                    printf("Error to draw a triangle!!!\n");
                }
            } else {
                free(instr_str);
                instr_str = NULL;
                    
                return EXIT_FAILURE;
            }
        } else if (instr_str[0] == 'f') {
            int32_t pos_y = 0, pos_x = 0;
            if (scanf("%d%d", &pos_y, &pos_x) > 2) {
                return EXIT_FAILURE;
            }

            fill_map(bmp, pos_y, pos_x);
        } else if (instr_str[0] == 'q') {
            quit_var = 0;

            free(instr_str);
            instr_str = NULL;

            quit_map(bmp);
            bmp = NULL;
        } else {
            free(instr_str);
            instr_str = NULL;
                    
            return EXIT_FAILURE;
        }
    }

    return 0;
}