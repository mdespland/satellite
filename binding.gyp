{
  "targets": [{
    "target_name": "mlx90641",
    "sources": [
      "mlx90641/mlx90641.cc",
      "mlx90641/mlx90641-driver/MLX90641_API.cpp", 
      "mlx90641/mlx90641-driver/MLX90641_LINUX_I2C_Driver.cpp"
    ],
    "include_dirs": [
      "<!@(node -p \"require('node-addon-api').include\")",
      "./mlx90641/mlx90641-driver"
    ],
    'defines': [ 'NAPI_CPP_EXCEPTIONS' ],
    'conditions': [
      [ 'OS=="win"', {
        'defines': [ '_HAS_EXCEPTIONS=1' ],
        'msvs_settings': {
          'VCCLCompilerTool': {
            'ExceptionHandling': 1,
          },
        },
      }],
      [ 'OS=="linux"', {
        "cflags": [ "-fexceptions" ],
        "cflags_cc": [ "-fexceptions" ],
      }],
      [ 'OS=="mac"', {
        'xcode_settings': {
          'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
          'CLANG_CXX_LIBRARY': 'libc++',
        }
      }]
    ],
  }]
}