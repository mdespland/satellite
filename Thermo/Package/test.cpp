#include <stdint.h>
#include <iostream>
#include <cstring>
#include <fstream>

#include "mlx90641.h"

int main(void) {

    MLX90641 * mlx90641=new MLX90641();
    uint32_t * pixels;
    pixels=mlx90641->getImage();
    for(int y = 0; y < SENSOR_H; y++){
        for(int x = 0; x < SENSOR_W; x++){
            int offset = (y * SENSOR_W + x);
            printf(" %d,",pixels[offset]);
        }
    }

    printf("]\n");
    printf("Max temp=%d\n",mlx90641->getMaxTemp());
    printf("Min temp=%d\n",mlx90641->getMinTemp());
}