cmd_Release/obj.target/addon.node := g++ -o Release/obj.target/addon.node -shared -pthread -rdynamic  -Wl,-soname=addon.node -Wl,--start-group Release/obj.target/addon/addon.o Release/obj.target/addon/mlx90641-driver/MLX90641_API.o Release/obj.target/addon/mlx90641-driver/MLX90641_LINUX_I2C_Driver.o -Wl,--end-group -lnode
