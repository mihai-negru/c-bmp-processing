#include "./include/bmp.h"

int main(void) {
    int quit_var = 1;

    /* Allocate a string buffer */
    char *instr_str = malloc(INSTR_MAX_LEN);

    if (instr_str == NULL) {
        return EXIT_FAILURE;
    }

    /* Allocate the bmp object */
    bmp_obj *bmp = malloc(sizeof(*bmp));

    if (bmp == NULL) {
        free(instr_str);
        instr_str = NULL;

        return EXIT_FAILURE;
    }

    /* Initialize the brush color and size */
    bmp->brush_color = malloc(BRUSH_MAX_LEN);
    bmp->brush_size = 1;
    
    if (bmp->brush_color == NULL) {
        free(instr_str);
        instr_str = NULL;

        return EXIT_FAILURE;
    }

    /* Start app loop */
    while (quit_var == 1) {
        
        /* Read first the instruction */
        if (scanf("%s", instr_str) > 1) {
            return EXIT_FAILURE;
        }

        if (instr_str[0] == 's') {
            if (instr_str[1] == 'a') {

                /* Instruction to save the bmp image */

                if (scanf("%s", instr_str) > 1) {
                    return EXIT_FAILURE;
                }

                if (save_map(instr_str, bmp) != 0) {
                    printf("Error to save!!!\n");
                }
            } else if (instr_str[1] == 'e') {

                /* Instruction to set a new brush color */

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

                    /* Instruction to set a new brush size */

                    uint8_t line_width = 1;
                    
                    if (scanf("%hhu", &line_width) > 1) {
                        return EXIT_FAILURE;
                    }

                    set_line_width(bmp, line_width);
                } else {

                    /* Unknown instruction */
                    free(instr_str);
                    instr_str = NULL;

                    return EXIT_FAILURE;
                }
            } else {

                /* Unknown instruction */
                free(instr_str);
                instr_str = NULL;
                    
                return EXIT_FAILURE;
            }
        } else if (instr_str[0] == 'e') {

            /* Instruction to load a bmp image into the program */

            if (scanf("%s", instr_str) > 1) {
                return EXIT_FAILURE;
            }

            edit_map(instr_str, bmp);

            if (bmp->img == NULL) {
                return EXIT_FAILURE;
            }
        } else if (instr_str[0] == 'i') {

            /* Instruction to overlap a new image on the loaded one */

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

                /* Instruction to draw a line in the bmp */

                int32_t pos_y1 = 0, pos_x1 = 0, pos_y2 = 0, pos_x2 = 0;

                if (scanf("%d%d%d%d", &pos_y1, &pos_x1, &pos_y2, &pos_x2) > 4) {
                    return EXIT_FAILURE;
                }

                if (draw_line(bmp, pos_y1, pos_x1, pos_y2, pos_x2) != 0) {
                    printf("Error to draw a line!!!\n");
                }
            } else if (instr_str[0] == 'r') {

                /* Instruction to draw a rectangle in the bmp */

                int32_t pos_y1 = 0, pos_x1 = 0, width_rec = 0, height_rec = 0;
                
                if (scanf("%d%d%d%d", &pos_y1, &pos_x1, &width_rec, &height_rec) > 4) {
                    return EXIT_FAILURE;
                }

                if (draw_rectangle(bmp, pos_y1, pos_x1, width_rec, height_rec) != 0) {
                    printf("Error to draw a rectangle!!!\n");
                }
            } else if (instr_str[0] == 't') {

                /* Instruction to draw a triangle in the bmp */

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

                /* Unknown instruction */
                free(instr_str);
                instr_str = NULL;
                    
                return EXIT_FAILURE;
            }
        } else if (instr_str[0] == 'f') {

            /* Instruction to fill with color a region */

            int32_t pos_y = 0, pos_x = 0;
            if (scanf("%d%d", &pos_y, &pos_x) > 2) {
                return EXIT_FAILURE;
            }

            fill_map(bmp, pos_y, pos_x);
        } else if (instr_str[0] == 'q') {

            /* Instruction to quit the program and to free memory */

            quit_var = 0;

            free(instr_str);
            instr_str = NULL;

            quit_map(bmp);
            bmp = NULL;
        } else {

            /* Unknown instruction */
            free(instr_str);
            instr_str = NULL;
                    
            return EXIT_FAILURE;
        }
    }

    return 0;
}