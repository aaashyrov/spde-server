//
// Created by alisher on 13.02.2021.
//

#ifndef SPDE_SERVER_INCLUDE_SPDE_SERVER_HPP_
#define SPDE_SERVER_INCLUDE_SPDE_SERVER_HPP_
#include <grpc++/grpc++.h>
#include <spde.pb.h>
#include <spde.grpc.pb.h>

class SpdeServer : public spde::Spde::Service {
 public:

  explicit SpdeServer(const std::string &endpoint);
  void run();
  ~SpdeServer() override;
 private:

  ::grpc::Status solve(::grpc::ServerContext *context,
                       const ::spde::Request *request,
                       ::spde::Response *response) override;


  grpc::ServerBuilder builder_;
  std::unique_ptr<grpc::Server> server_;
  std::string endpoint_;

};

#endif //SPDE_SERVER_INCLUDE_SPDE_SERVER_HPP_
