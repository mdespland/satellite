{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "testcpp/addon.cc" , "testcpp/mlx90641-driver/MLX90641_API.cpp", "testcpp/mlx90641-driver/MLX90641_LINUX_I2C_Driver.cpp"],
      'include_dirs' : [
        "testcpp/mlx90641-driver"
      ],
    }
  ]
}