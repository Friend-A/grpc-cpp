cmake_minimum_required(VERSION 3.5.1)

project(helloworld) # 定义了项目的名称

set(Protobuf_DIR "/usr/local/lib64/cmake/protobuf")  # 指定安装路径
set(gRPC_DIR "/usr/local/lib/cmake/grpc")            # 指定安装路径

find_package(Protobuf CONFIG REQUIRED)              
find_package(gRPC CONFIG REQUIRED)
# 这两行指示CMake查找安装在系统上的 gRPC 和 Protobuf（协议缓冲区）库。CONFIG REQUIRED 表示这些库是必须的，如果找不到，CMake将停止执行并报错

find_package(Threads REQUIRED)

add_library(protos helloworld.pb.cc helloworld.grpc.pb.cc)
target_link_libraries(protos PUBLIC gRPC::grpc++)

add_executable(greeter_server greeter_server.cc)
target_link_libraries(greeter_server PRIVATE protos gRPC::grpc++ Threads::Threads)

add_executable(greeter_client greeter_client.cc)
target_link_libraries(greeter_client PRIVATE protos gRPC::grpc++ Threads::Threads)

