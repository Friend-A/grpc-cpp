#ifndef MONITORING_INTERCEPTOR_H
#define MONITORING_INTERCEPTOR_H

#include <grpcpp/grpcpp.h>
#include <chrono>
#include <atomic>
#include <iostream>
#include <thread>

// 监控拦截器，用于统计QPS和耗时
class MonitoringInterceptor : public grpc::experimental::Interceptor {
public:
    MonitoringInterceptor() = default;

    void Intercept(grpc::experimental::InterceptorBatchMethods* methods) override {
        if (methods->QueryInterceptionHookPoint(grpc::experimental::InterceptionHookPoints::POST_RECV_MESSAGE)) {
            // 请求开始，记录开始时间
            start_time_ = std::chrono::high_resolution_clock::now();
            // 打印开始时间
            // 使用 strftime 格式化并打印开始时间
            std::time_t start_time = std::chrono::system_clock::to_time_t(start_time_);
            char start_time_str[100];
            std::strftime(start_time_str, sizeof(start_time_str), "%Y-%m-%d %H:%M:%S", std::localtime(&start_time));
            std::cout << "Start time: " << start_time_str << std::endl;
            
            qps_counter_++;
        }

        if (methods->QueryInterceptionHookPoint(grpc::experimental::InterceptionHookPoints::POST_SEND_MESSAGE)) {
            // 请求结束，计算耗时
            auto end = std::chrono::high_resolution_clock::now();
            std::time_t end_time = std::chrono::system_clock::to_time_t(end);
            // 使用 strftime 格式化并打印结束时间
            char end_time_str[100];
            std::strftime(end_time_str, sizeof(end_time_str), "%Y-%m-%d %H:%M:%S", std::localtime(&end_time));
            std::cout << "End time: " << end_time_str << std::endl;

            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start_time_);
            std::cout << "RPC call took " << duration.count() << " ms" << std::endl;
        }

        methods->Proceed();
    }
    /* 常用的拦截点：
        POST_RECV_INITIAL_METADATA: 接收初始元数据之后。
        POST_RECV_MESSAGE: 接收消息之后。
        POST_SEND_MESSAGE: 发送消息之后。
        POST_SEND_INITIAL_METADATA: 发送初始元数据之后。
        POST_RECV_STATUS_ON_CLIENT: 客户端接收状态码时。
        POST_SEND_CLOSE_FROM_CLIENT: 客户端发送关闭请求时。
        
        这些拦截点根据 gRPC 的生命周期触发，可以根据需求选择合适的拦截点来统计性能数据等 

        PRE_SEND_INITIAL_METADATA：在发送初始元数据之前触发，通常可以视为请求开始的时刻（适合记录请求的开始时间）。
        POST_RECV_MESSAGE：在接收到请求消息之后触发，表示服务器收到了客户端发送的完整消息。
        POST_SEND_MESSAGE：在发送响应消息之后触发，可以用于计算发送响应的时间点。
        POST_SEND_TRAILING_METADATA：在发送尾随元数据之后触发，通常表示一个 gRPC 请求的生命周期即将结束。
    */

    // 静态方法，每秒输出一次QPS
    static void MonitorQps() {
        while (true) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            int qps = qps_counter_.exchange(0);
            std::cout << "QPS: " << qps << std::endl;
        }
    }

private:
    std::chrono::high_resolution_clock::time_point start_time_;
    static std::atomic<int> qps_counter_;
};

std::atomic<int> MonitoringInterceptor::qps_counter_{0};

#endif  // MONITORING_INTERCEPTOR_H
