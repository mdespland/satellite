#/*MIT License
#Copyright (c) 2018 imliubo
#Github  https://github.com/imliubo
#Website https://www.makingfun.xyz
#Permission is hereby granted, free of charge, to any person obtaining a copy
#of this software and associated documentation files (the "Software"), to deal
#in the Software without restriction, including without limitation the rights
#to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#copies of the Software, and to permit persons to whom the Software is
#furnished to do so, subject to the following conditions:
#The above copyright notice and this permission notice shall be included in all
#copies or substantial portions of the Software.
#THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
#SOFTWARE.
#*/
CC = g++
CXXFLAGS = -O -g -lSDL2main -lSDL2 -I/usr/include/SDL2 -D_REENTRANT
LFLAGS=-lSDL2main -lSDL2 -lSDL2_ttf
OBJS = main.o mlx90641.o libMLX90641_API.a
INCLUDE += -I ./Include -I ./Include/mlx90641-driver
TARGET = main

vpath %.cpp ./Source ./Source/mlx90641-driver
vpath %.c ./Source ./Source/mlx90641-driver

all:main.o mlx90641.o libMLX90641_API.a
	$(CC) $(CXXFLAGS) $(INCLUDE) -o $(TARGET) $(OBJS) $(LFLAGS)
	rm -f *.o *a
	
main.o:main.cpp 
	$(CC) $(CXXFLAGS) $(LFLAGS) $(INCLUDE) -c $^

libMLX90641_API.a:MLX90641_API.o MLX90641_LINUX_I2C_Driver.o
	ar rcs $@ $^
	ranlib $@

mlx90641.o:mlx90641.cpp
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $^

MLX90641_API.o:MLX90641_API.cpp
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $^

MLX90641_LINUX_I2C_Driver.o:MLX90641_LINUX_I2C_Driver.cpp
	$(CC) $(CXXFLAGS) $(INCLUDE) -c $^

.PHONY:clean
clean:
	rm -f *.o
	rm -f *.a
	rm -f $(TARGET)

