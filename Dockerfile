# 使用官方的 Ubuntu 基础镜像
FROM ubuntu:20.04

# 设置时区为非交互模式，避免 tzdata 的提示
ENV DEBIAN_FRONTEND=noninteractive

# 更新包列表并安装必要的软件包
RUN apt-get update && \
    apt-get install -y --no-install-recommends \
    build-essential \
    autoconf \
    libtool \
    pkg-config \
    cmake \
    git \
    curl \
    vim \
    ca-certificates \
    zlib1g-dev \
    libc-ares-dev \
    libssl-dev \
    libgflags-dev \
    libprotobuf-dev \
    protobuf-compiler \
    unzip \
    && rm -rf /var/lib/apt/lists/*


# 克隆 gRPC 仓库, 经验证，该环境可以使用 1.57.0
#WORKDIR /opt
#RUN git clone --recurse-submodules -b v1.57.0 https://github.com/grpc/grpc.git
COPY build_tmp/grpc /opt/grpc

# 进入 gRPC 目录并构建 gRPC 和 Protobuf
WORKDIR /opt/grpc
RUN mkdir -p cmake/build && cd cmake/build && \
    cmake ../.. -DBUILD_SHARED_LIBS=ON && \
    make -j$(nproc) && \
    make install

# 删除构建中产生的临时文件
RUN apt-get clean && rm -rf /var/lib/apt/lists/* /tmp/* /var/tmp/*

# 设置环境变量
ENV LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
#ENV LD_LIBRARY_PATH=/usr/local/lib:${LD_LIBRARY_PATH:-}
ENV PATH=/usr/local/bin:$PATH

# 验证安装
RUN protoc --version

# 设置工作目录为用户的工作目录
WORKDIR /workspace

# 镜像最终的 CMD 可按需设置，以下为示例
CMD ["/bin/bash"]
