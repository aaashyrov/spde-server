//
// Created by alisher on 13.02.2021.
//

#ifndef SPDE_SERVER_INCLUDE_SPDE_CLIENT_HPP_
#define SPDE_SERVER_INCLUDE_SPDE_CLIENT_HPP_
#include <string>
#include <grpc++/grpc++.h>
#include <spde.grpc.pb.h>
class SpdeClient {
 public:

  explicit SpdeClient(const std::string &endpoint);
  ~SpdeClient() = default;

  void solve();
 private:
  std::unique_ptr<spde::Spde::Stub> stub_;
};

#endif //SPDE_SERVER_INCLUDE_SPDE_CLIENT_HPP_
