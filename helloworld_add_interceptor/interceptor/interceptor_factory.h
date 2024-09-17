#ifndef MONITORING_INTERCEPTOR_FACTORY_H
#define MONITORING_INTERCEPTOR_FACTORY_H

#include "monitoring_interceptor.h"

// 拦截器工厂，用于创建拦截器实例
class MonitoringInterceptorFactory : public grpc::experimental::ServerInterceptorFactoryInterface {
public:
    grpc::experimental::Interceptor* CreateServerInterceptor(grpc::experimental::ServerRpcInfo* info) override {
        return new MonitoringInterceptor();
    }
};

#endif  // MONITORING_INTERCEPTOR_FACTORY_H
