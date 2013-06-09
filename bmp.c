#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

typedef struct bmp_header {
    uint16_t magic;
    uint32_t size;
    uint16_t emp1;
    uint16_t emp2;
    uint32_t pix_offset;
} bmp_header;

typedef struct bmp_dib {
    uint32_t dibsize;
    uint32_t w;
    uint32_t h;
    uint16_t colorplanes;
    uint16_t bpp;
    uint32_t compression;
    uint32_t pix_size;
    uint32_t xres;
    uint32_t yres;
    uint32_t colors_in_palette;
    uint32_t color_importance;
} bmp_dib;

typedef struct bmp_file {
    bmp_header header;
    bmp_dib dib;
    uint8_t *pixels;
    uint32_t *color_table;
} bmp_file;

void read_bmp_header(bmp_file* bmp, FILE* f)
{
    bmp_header *head = &(bmp->header);
    fread(&(head->magic), 2, 1, f);
    fread(&(head->size), 4, 1, f);
    fread(&(head->emp1), 2, 1, f);
    fread(&(head->emp2), 2, 1, f);
    fread(&(head->pix_offset), 4, 1, f);
}

void read_bmp_dib(bmp_file* bmp, FILE* f)
{
    bmp_dib *dib = &(bmp->dib);
    fread(&(dib->dibsize), 4, 1, f);
    fread(&(dib->w), 4, 1, f);
    fread(&(dib->h), 4, 1, f);
    fread(&(dib->colorplanes), 2, 1, f);
    fread(&(dib->bpp), 2, 1, f);
    fread(&(dib->compression), 4, 1, f);
    fread(&(dib->pix_size), 4, 1, f);
    fread(&(dib->xres), 4, 1, f);
    fread(&(dib->yres), 4, 1, f);
    fread(&(dib->colors_in_palette), 4, 1, f);
    fread(&(dib->color_importance), 4, 1, f);
}

void read_bmp_pixels(bmp_file *bmp, FILE *f)
{
    int num_pxls = ceil(bmp->dib.w * 8 * sizeof(uint8_t)/32.0) * 4 * bmp->dib.h;
    bmp->pixels = malloc(num_pxls);
    fseek(f, bmp->header.pix_offset, SEEK_SET);
    printf("PXLOFF: %u", bmp->header.pix_offset);
    //FIX READING... ADD COLOR TABLE LOOKUP
    fread(bmp->pixels, num_pxls, 1, f);
}

void read_bmp_color_table(bmp_file *bmp, FILE *f)
{
    int size = 1 << (bmp->dib.bpp + 2);
    bmp->color_table = malloc(size);
    fread(bmp->color_table, size, 1, f);
}

bmp_file *read_bmp(char *filename)
{
    FILE *f = fopen(filename, "r");
    bmp_file *b = malloc(sizeof(bmp_file));
    read_bmp_header(b, f);
    read_bmp_dib(b, f);
    read_bmp_color_table(b, f);
    read_bmp_pixels(b, f);
    fclose(f);
    return b;
}

void free_bmp(bmp_file *bmp)
{
    free(bmp->pixels);
    free(bmp->color_table);
    free(bmp);
}

void* convert_bmp_to_pixels(bmp_file* bmp, int bytes)
{
    int size = bmp->dib.w * bmp->dib.h * sizeof(uint32_t);
    uint32_t *pixels = malloc(size);
    int bmpoff = 0;
    int pxloff = 0;
    int i, j;
    for(i = bmp->dib.h - 1; i >= 0; --i){
        for(j = 0; j < bmp->dib.w; ++j){
            pxloff = i * bmp->dib.w + j;
            pixels[pxloff] = bmp->color_table[bmp->pixels[bmpoff++]];
        }
        if (bmpoff % 4 != 0)
            bmpoff += 4 - (bmpoff % 4); //padding likely not needed since converted above to uint32_t
    }
    return pixels;
}

uint32_t get_bmp_width(bmp_file *bmp)
{
    return bmp->dib.w;
}

uint32_t get_bmp_height(bmp_file *bmp)
{
    return bmp->dib.h;
}

uint16_t get_bmp_bits_per_pixel(bmp_file *bmp)
{
    return bmp->dib.bpp;
}
