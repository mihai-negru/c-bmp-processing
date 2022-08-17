#ifndef BMP_HEADER_H_
#define BMP_HEADER_H_

#pragma pack(1)

/**
 * @brief Definition of a BMP file header
 * 
 */
typedef struct {
    uint8_t     file_mark1;                  /* 'B' */
    uint8_t     file_mark2;                  /* 'M' */
    uint32_t    bf_size;                     /* File's size */
    uint16_t    unused1;                     /* Aplication specific */
    uint16_t    unused2;                     /* Aplication specific */
    uint32_t    img_data_offset;            /* Offset to the start of image data */
} bmp_fileheader;

/**
 * @brief Definition of a BMP file information header
 * 
 */
typedef struct {
    uint32_t    bi_size;                     /* Size of the info header - 40 bytes */
    int32_t     width;                       /* Width of the image */
    int32_t     height;                      /* Height of the image */
    uint16_t    planes;
    uint16_t    bit_pix;                     /* Number of bits per pixel*/
    uint32_t    bi_compression;              /* Type of compression */
    uint32_t    bi_size_image;               /* Size of the image data */
    int32_t     bi_xpels_per_meter;
    int32_t     bi_ypels_per_meter;
    uint32_t    bi_clr_used;
    uint32_t    bi_clr_important;
} bmp_infoheader;

#pragma pack()

#endif /* BMP_HEADER_H_ */