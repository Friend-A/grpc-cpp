cmake_minimum_required(VERSION 3.5.1)

project(helloworld) # 定义了项目的名称

set(Protobuf_DIR "/usr/local/lib64/cmake/protobuf")  # 指定安装路径
set(gRPC_DIR "/usr/local/lib/cmake/grpc")            # 指定安装路径

find_package(Protobuf CONFIG REQUIRED)              
find_package(gRPC CONFIG REQUIRED)
# 这两行指示CMake查找安装在系统上的 gRPC 和 Protobuf（协议缓冲区）库。CONFIG REQUIRED 表示这些库是必须的，如果找不到，CMake将停止执行并报错
find_package(Threads REQUIRED)

# 查找 grpc_cpp_plugin
find_program(GRPC_CPP_PLUGIN grpc_cpp_plugin)
if(NOT GRPC_CPP_PLUGIN)
    message(FATAL_ERROR "grpc_cpp_plugin not found")
endif()
message(STATUS "Found grpc_cpp_plugin: ${GRPC_CPP_PLUGIN}")
# 相当于 which grpc_cpp_plugin
# `which grpc_cpp_plugin` 在 add_custom_command 或者是 cmake中, 不生效

# 查找 protoc
find_program(Protobuf_PROTOC_EXECUTABLE protoc)
if(NOT Protobuf_PROTOC_EXECUTABLE)
    message(FATAL_ERROR "protoc not found")
endif()
message(STATUS "Found protoc: ${Protobuf_PROTOC_EXECUTABLE}")

# 设置 proto 文件目录和生成文件输出目录
set(PROTO_FILES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/protos)
message(STATUS "设置源proto文件的目录: ${PROTO_FILES_DIR}")

set(GENERATED_PROTO_DIR ${CMAKE_CURRENT_BINARY_DIR}/generated)
message(STATUS "设置生成的pb文件目录: ${GENERATED_PROTO_DIR}")

# 创建生成目录
file(MAKE_DIRECTORY ${GENERATED_PROTO_DIR})

# 查找所有的 proto 文件
file(GLOB PROTO_FILES "${PROTO_FILES_DIR}/*.proto")

# 为每个 proto 文件生成 C++ 源文件
foreach(PROTO_FILE ${PROTO_FILES})
    get_filename_component(PROTO_FILENAME ${PROTO_FILE} NAME_WE)

    # 定义生成的文件路径
    set(GENERATED_PROTO_SRC ${GENERATED_PROTO_DIR}/${PROTO_FILENAME}.pb.cc)
    set(GENERATED_PROTO_HDR ${GENERATED_PROTO_DIR}/${PROTO_FILENAME}.pb.h)
    set(GENERATED_GRPC_SRC ${GENERATED_PROTO_DIR}/${PROTO_FILENAME}.grpc.pb.cc)
    set(GENERATED_GRPC_HDR ${GENERATED_PROTO_DIR}/${PROTO_FILENAME}.grpc.pb.h)

    # 添加自定义命令来生成 .pb.cc 和 .grpc.pb.cc 文件
    add_custom_command(
        OUTPUT ${GENERATED_PROTO_SRC} ${GENERATED_PROTO_HDR} ${GENERATED_GRPC_SRC} ${GENERATED_GRPC_HDR}
        COMMAND ${Protobuf_PROTOC_EXECUTABLE}
        ARGS --grpc_out=${GENERATED_PROTO_DIR}
             --cpp_out=${GENERATED_PROTO_DIR}
             --proto_path=${PROTO_FILES_DIR}
             --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN}
             ${PROTO_FILE}
        DEPENDS ${PROTO_FILE}
    )

    # 添加生成的源文件到 proto 库
    list(APPEND PROTO_SRCS ${GENERATED_PROTO_SRC} ${GENERATED_GRPC_SRC})
    list(APPEND PROTO_HDRS ${GENERATED_PROTO_HDR} ${GENERATED_GRPC_HDR})
endforeach()

include_directories(${GENERATED_PROTO_DIR})

# 添加 proto 库
add_library(protos ${PROTO_SRCS} ${PROTO_HDRS})
target_link_libraries(protos PUBLIC gRPC::grpc++)

# 拦截器源文件目录
set(INTERCEPTOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/interceptor")
# 自动添加 interceptor 目录中的所有 .cpp 文件
file(GLOB INTERCEPTOR_SOURCES "${INTERCEPTOR_DIR}/*.cpp")
# 包含路径
include_directories(${INTERCEPTOR_DIR})

# 设置可执行文件输出目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# 添加可执行文件
add_executable(greeter_server src/greeter_server.cc)
target_link_libraries(greeter_server PRIVATE protos gRPC::grpc++ grpc++_reflection Threads::Threads)

add_executable(greeter_client src/greeter_client.cc)
target_link_libraries(greeter_client PRIVATE protos gRPC::grpc++ Threads::Threads)

#[[
1. 关于在代码中  #include "xxx.grpc.pb.h" 等报错没有头文件：使用include_directories

    include_directories(${GENERATED_PROTO_DIR}) 通常放在 CMake 文件中声明源文件或添加目标之前。这样可以确保在 CMake 开始处理源文件和目标之前，头文件搜索路径已经正确设置。
    放置位置的建议
    1.项目级别的 include_directories:
    如果 GENERATED_PROTO_DIR 是项目中所有目标共享的头文件目录，可以将 include_directories(${GENERATED_PROTO_DIR}) 放在 CMakeLists.txt 中较前面的位置，例如在 add_executable 或 add_library 之前。
    这样可以确保所有后续的目标都能使用这个路径。
    2.特定目标的 include_directories:
    如果只希望某个特定的目标使用这个路径，可以在定义该目标的 add_executable 或 add_library 之前或之后使用 target_include_directories。例如：
        add_executable(my_executable main.cpp ${PROTO_SRCS})
        target_include_directories(my_executable PRIVATE ${GENERATED_PROTO_DIR})
    这种方式更精细，可以为不同的目标设置不同的头文件搜索路径。
]]