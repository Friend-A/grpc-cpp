#include <iostream>
#include <memory>
#include <string>

#include <grpcpp/grpcpp.h>
#include <grpcpp/ext/proto_server_reflection_plugin.h>

#include "helloworld.grpc.pb.h"
#include "monitoring_interceptor.h"
#include "interceptor_factory.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using helloworld::Greeter;
using helloworld::HelloReply;
using helloworld::HelloRequest;

class GreeterServiceImpl final : public Greeter::Service {
  Status SayHello(ServerContext* context, const HelloRequest* request,
                  HelloReply* reply) override {
    std::string prefix("Hello ");
    reply->set_message(prefix + request->name());
    sleep(3);
    return Status::OK;
  }
};

void RunServer() {
  // std::string server_address("0.0.0.0:50051");
  std::string server_address("0.0.0.0:8081");
  GreeterServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();

  ServerBuilder builder;
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  builder.RegisterService(&service);

  // 创建不同的拦截器，并传递给通用拦截器工厂
  std::vector<std::unique_ptr<grpc::experimental::ServerInterceptorFactoryInterface>> creators;
  creators.push_back(std::unique_ptr<grpc::experimental::ServerInterceptorFactoryInterface>(
      new MonitoringInterceptorFactory()));
  builder.experimental().SetInterceptorCreators(std::move(creators));

  
  // std::vector<std::function<grpc::experimental::Interceptor*()>> interceptors = {
  //     []() { return new MonitoringInterceptor(); }
  //     // 在这里添加其他拦截器，例如：
  //     // []() { return new AuthenticationInterceptor(); }
  // };
  // // 使用 SetInterceptorCreators 来设置拦截器
  //   builder.experimental().SetInterceptorCreators({
  //       []() {
  //           return std::unique_ptr<grpc::experimental::Interceptor>(new MonitoringInterceptor());
  //       }
  //   });

  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();
  return 0;
}

