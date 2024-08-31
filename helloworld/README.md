1. 根据proto文件生成相关的文件：
```
protoc -I . --grpc_out=. --plugin=protoc-gen-grpc=`which grpc_cpp_plugin` helloworld.proto
protoc -I . --cpp_out=. helloworld.proto
```
2. 编译运行服务端，客户端
```
mkdir -p build
cd build
cmake ..
make -j$(nproc)
./greeter_server & ./greeter_client
```
