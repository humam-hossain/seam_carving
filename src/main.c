#include <assert.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <float.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

static float gx[3][3] = {
    {1.0, 0.0, -1.0},
    {2.0, 0.0, -2.0},
    {1.0, 0.0, -1.0}
};
static float gy[3][3] = {
    {1.0, 2.0, 1.0},
    {0.0, 0.0, 0.0},
    {-1.0, -2.0, -1.0}
};

float rgb_to_lum(uint32_t rgb)
{
    float r = ((rgb >> (8*0)) & 0xFF)/255.0;
    float g = ((rgb >> (8*1)) & 0xFF)/255.0;
    float b = ((rgb >> (8*2)) & 0xFF)/255.0;

    return 0.2126*r + 0.7152*g + 0.0722*b;
}

void analyze_min_max(const char *prompt, float *mat, int width, int height)
{
    float min = FLT_MAX;
    float max = FLT_MIN;

    for(int i=0; i<width*height; ++i){
        if(mat[i] < min) min = mat[i];
        if(mat[i] > max) max = mat[i];
    }

    printf("%s: min=%f, max=%f\n", prompt, min, max);
}

int main()
{
    const char *file_path = "assets/Broadway_tower_edit.jpg";

    int width, height;
    uint32_t *pixels = (uint32_t*) stbi_load(file_path, &width, &height, NULL, 4);
    if(pixels == NULL){
        fprintf(stderr, "[ERROR] could not read %s\n", file_path);
        return 1;
    }

    printf("file path: %s\n", file_path);
    printf("width: %d\n", width);
    printf("height: %d\n", height);

    float *lum = malloc(sizeof(*lum)*width*height);
    assert(lum != NULL);
    for(int i=0; i<width*height; ++i){
        lum[i] = rgb_to_lum(pixels[i]);
    }
    analyze_min_max("lum", lum, width, height);

    float *grad = malloc(sizeof(*grad)*width*height);
    assert(grad != NULL);
    for(int cy=0; cy<height; ++cy){
        for(int cx=0; cx<width; ++cx){
            float sx = 0.0;
            float sy = 0.0;
            for(int dy=-1; dy<=1; ++dy){
               	for(int dx=-1; dx<=1; ++dx){
             	  int x = cx + dx;
             	  int y = cy + dy;
             	  float c = 0<=x && x<width && 0<=y && y<height ? lum[width*y + x] : 0.0f;
             	  sx += c*gx[dy+1][dx+1];
             	  sy += c*gy[dy+1][dx+1];
               	}
            }
            grad[width*cy + cx] = sqrtf(sx*sx+sy*sy);
        }
    }
    analyze_min_max("grad", grad, width, height);

    // for(int i=0; i<width*height; ++i){
    //     uint32_t value = 255*lum[i];
    //     pixels[i] = 0xFF000000|(value<<(8*2))|(value<<(8*1))|(value<<(8*0));
    // }

    // const char *lum_file_path = "assets/lum.png";
    // if(!stbi_write_png(lum_file_path, width, height, 4, pixels, width*sizeof(*pixels))){
    //     fprintf(stderr, "[ERROR] could not save file %s", lum_file_path);
    //     return 1;
    // }

    printf("OK\n");

    return 0;
}
