cmd_Release/obj.target/addon/addon.o := g++ -o Release/obj.target/addon/addon.o ../addon.cc '-DNODE_GYP_MODULE_NAME=addon' '-DUSING_UV_SHARED=1' '-DUSING_V8_SHARED=1' '-DV8_DEPRECATION_WARNINGS=1' '-DV8_DEPRECATION_WARNINGS' '-DV8_IMMINENT_DEPRECATION_WARNINGS' '-D_LARGEFILE_SOURCE' '-D_FILE_OFFSET_BITS=64' '-D__STDC_FORMAT_MACROS' '-DNAPI_CPP_EXCEPTIONS' '-DBUILDING_NODE_EXTENSION' -I/usr/include/nodejs/include/node -I/usr/include/nodejs/src -I/usr/include/nodejs/deps/openssl/config -I/usr/include/nodejs/deps/openssl/openssl/include -I/usr/include/nodejs/deps/uv/include -I/usr/include/nodejs/deps/zlib -I/usr/include/nodejs/deps/v8/include -I/home/mde/Satellite/addon/node_modules/node-addon-api -I../mlx90641-driver  -fPIC -pthread -Wall -Wextra -Wno-unused-parameter -fPIC -fexceptions -O3 -fno-omit-frame-pointer -fno-rtti -fno-exceptions -std=gnu++1y -fexceptions -MMD -MF ./Release/.deps/Release/obj.target/addon/addon.o.d.raw   -c
Release/obj.target/addon/addon.o: ../addon.cc \
 /home/mde/Satellite/addon/node_modules/node-addon-api/napi.h \
 /usr/include/nodejs/src/node_api.h \
 /usr/include/nodejs/src/js_native_api.h \
 /usr/include/nodejs/src/js_native_api_types.h \
 /usr/include/nodejs/src/node_api_types.h \
 /home/mde/Satellite/addon/node_modules/node-addon-api/napi-inl.h \
 /home/mde/Satellite/addon/node_modules/node-addon-api/napi-inl.deprecated.h \
 ../worker.h ../mlx90641.h ../mlx90641-driver/MLX90641_API.h
../addon.cc:
/home/mde/Satellite/addon/node_modules/node-addon-api/napi.h:
/usr/include/nodejs/src/node_api.h:
/usr/include/nodejs/src/js_native_api.h:
/usr/include/nodejs/src/js_native_api_types.h:
/usr/include/nodejs/src/node_api_types.h:
/home/mde/Satellite/addon/node_modules/node-addon-api/napi-inl.h:
/home/mde/Satellite/addon/node_modules/node-addon-api/napi-inl.deprecated.h:
../worker.h:
../mlx90641.h:
../mlx90641-driver/MLX90641_API.h:
