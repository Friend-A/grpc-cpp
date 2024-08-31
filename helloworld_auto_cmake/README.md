1. 编译运行服务端，客户端
```
mkdir -p build
cd build
cmake ..
make -j$(nproc)
./greeter_server &

// 查看是否开启端口

// 运行客户端请求
./greeter_client
```
