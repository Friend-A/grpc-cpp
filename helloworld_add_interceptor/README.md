# 添加拦截器
gRPC 支持拦截器机制，可以在调用的前后执行自定义的逻辑。你可以利用拦截器来监控每个 RPC 的耗时、QPS 等指标。

## 例如在服务端添加拦截器，统计每个请求的耗时
1. 创建自定义的 Interceptor 类，在 RPC 调用的不同阶段记录时间
```C++
class MonitoringInterceptor : public grpc::experimental::Interceptor

// 在 PRE_SEND_INITIAL_METADATA 的时候计算开始时间：
//     PRE_SEND_INITIAL_METADATA：在发送初始元数据之前触发，通常可以视为请求开始的时刻（适合记录请求的开始时间）。
// 在 POST_SEND_MESSAGE 的时候计算结束时间
//     POST_SEND_MESSAGE：在发送响应消息之后触发，可以用于计算发送响应的时间点。

// 表示该服务端接口处理该请求的耗时

```
2. 创建该拦截器的工厂类，用于创建拦截器
```C++
// 拦截器工厂，用于创建拦截器实例
class MonitoringInterceptorFactory : public grpc::experimental::ServerInterceptorFactoryInterface {
public:
    grpc::experimental::Interceptor* CreateServerInterceptor(grpc::experimental::ServerRpcInfo* info) override {
        return new MonitoringInterceptor();
    }
};

```
3. 添加到服务端中
```C++
std::vector<std::unique_ptr<grpc::experimental::ServerInterceptorFactoryInterface>> creators;
creators.push_back(std::unique_ptr<grpc::experimental::ServerInterceptorFactoryInterface>(
    new MonitoringInterceptorFactory()));
builder.experimental().SetInterceptorCreators(std::move(creators));

```
4. cmake添加
```cmake
# 拦截器源文件目录
set(INTERCEPTOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/interceptor")
# 自动添加 interceptor 目录中的所有 .cpp 文件
file(GLOB INTERCEPTOR_SOURCES "${INTERCEPTOR_DIR}/*.cpp")
# 包含路径
include_directories(${INTERCEPTOR_DIR})
```


# 如何编译运行
1. 编译运行服务端，客户端
```shell
mkdir -p build
cd build
cmake ..
make -j$(nproc)
./greeter_server &

// 查看是否开启端口

// 运行客户端请求
./greeter_client
```

2. 结果：
```shell
./greeter_server 
Server listening on 0.0.0.0:50051
Start time: 2024-09-17 22:05:20
End time: 2024-09-17 22:05:20
RPC call took 0 ms

Start time: 2024-09-17 22:05:45
End time: 2024-09-17 22:05:45
RPC call took 1 ms
```
```shell
./greeter_client 
Greeter received: Hello world
```