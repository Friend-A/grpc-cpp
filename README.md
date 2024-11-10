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

## 四. 添加拦截器例子
如代码 helloworld_add_interceptor 所示
├── CMakeLists.txt
├── interceptor
│   ├── interceptor_factory.h
│   └── monitoring_interceptor.h
├── protos
│   └── helloworld.proto
├── README.md
└── src
    ├── greeter_client.cc
    └── greeter_server.cc

## 五. 添加reflection
```
安装grpc curl
export GOPROXY=https://mirrors.aliyun.com/goproxy/
go install github.com/fullstorydev/grpcurl/cmd/grpcurl

copy bin/grpcurl 到 /usr/local/bin
```
使用grpcurl验证：
```
grpcurl -plaintext 127.0.0.1:8081 list
grpcurl -plaintext localhost:8081 describe helloworld.Greeter.SayHello
grpcurl -plaintext -d '{ "name": "9527" }' localhost:8081 helloworld.Greeter.SayHello
```


## 五. 服务端注册多个服务

## 六. 优化和配置: 
安全性: 使用 SSL/TLS 认证  
日志记录: 集成日志记录系统如 spdlog, zlog  
性能优化: 使用多线程处理请求，通过负载均衡扩展服务  
错误处理: 处理 gRPC 状态码和异常  
Docker 部署: 创建 Dockerfile 并配置容器化部署  
启用 Reflection，使用postman或者grpc-cli等工具可以直接列出开放了哪些grpc服务接口
客户端连接复用：复用了相同的 Channel 实例  

异步 I/O: 使用 gRPC 的异步 API 可以减少阻塞操作，提高并发处理能力  
缓存: 引入缓存机制减少对下游服务的请求，减轻负载  
数据压缩: 使用 gRPC 的内置数据压缩功能减少传输的数据量   
连接复用: 配置 gRPC 复用 TCP 连接，减少握手和连接建立的开销  

## 会不停优化，并且添加更多示例，欢迎加star

## 有什么疑问、建议、优化，或者需求，可以一起讨论，欢迎star并留言
