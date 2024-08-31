# grpc-cpp
Build a grpc-cpp environment

## 一. Dockerfile
由于grpc cpp环境编译与构建不易，故提供已调试好的 Dockerfile，以便快速构建环境。

## 二. helloworld
一个简单的例子，验证grpc是否可以跑起来。包括
1. 根据proto文件生成代码；
2. 使用cmake编译grpc客户端服务端

## 三. helloworld_auto_cmake
进一步探索，将proto文件放在一个目录，通过cmake编译代码的时候，自动生成pb文件，而不是手动去生成。
```
helloworld_auto_cmake/
├── CMakeLists.txt
├── src/
│   ├── greeter_client.cc
│   ├── greeter_server.cc
└── protos/
    └── helloworld.proto
```
好处就是可以将很多proto文件，生成很多pb文件，而不用手动一个一个生成。

## 会不停优化，并且添加更多示例，欢迎加star

## 有什么疑问、建议、优化，或者需求，可以一起讨论，欢迎star并留言