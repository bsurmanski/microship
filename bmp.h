typedef struct bmp_file bmp_file;

struct bmp_file *read_bmp(char *filename);
void free_bmp(bmp_file* bmp);
void* convert_bmp_to_pixels(struct bmp_file *bmp, int bytes);
uint32_t get_bmp_width(bmp_file *bmp);
uint32_t get_bmp_height(bmp_file *bmp);
uint16_t get_bmp_bits_per_pixel(bmp_file *bmp);
