CFLAGS += -c -g -Wall -Wextra -Wpedantic \
                  -Wformat=2 -Wno-unused-parameter \
                  -Wshadow -Wwrite-strings -Wstrict-prototypes \
                  -Wold-style-definition -Wredundant-decls \
                  -Wnested-externs -Wmissing-include-dirs \
                  -Wjump-misses-init -Wlogical-op -O2

PATH_TO_FILES += ../src
PATH_TO_HEADER_FILES += $(PATH_TO_FILES)/include

FILES += $(PATH_TO_FILES)/basic_commands.c $(PATH_TO_FILES)/draw_commands.c \
		 $(PATH_TO_FILES)/insert_image.c $(PATH_TO_FILES)/fill_color.c \
		 $(PATH_TO_FILES)/bmp.c

build: bmp
	@rm -rf *.o

bmp: bmp_obj_files
	@gcc *.o -o bmp

bmp_obj_files: $(FILES)
	@gcc $(CFLAGS) $(FILES)

clean:
	@find . -type f -name "*.o" -exec rm -rf {} \;
	@rm -rf output bmp