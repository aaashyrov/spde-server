//
// Created by alisher on 13.02.2021.
//

#include "../include/spde_server.hpp"

SpdeServer::SpdeServer(const std::string &endpoint) {
  builder_.AddListeningPort(endpoint_ = endpoint, grpc::InsecureServerCredentials());
  builder_.RegisterService(this);
}

void SpdeServer::run() {
  std::cout << "[spde]: processing on " << endpoint_ << std::endl;
  server_ = builder_.BuildAndStart();
  server_->Wait();
}

void validate(const ::spde::Request *request);

::grpc::Status SpdeServer::solve(::grpc::ServerContext *context,
                                 const ::spde::Request *request, ::spde::Response *response) {

  auto fail = [](std::exception &ex) {
    std::cerr << ex.what() << std::endl;
    return grpc::Status(grpc::ABORTED, ex.what());
  };

  try {
    validate(request);

  } catch (std::exception &ex) {
    return fail(ex);
  }
}

void validate(const ::spde::Request *request) {
  if (request->function().x().empty() or request->function().q().empty() or request->function().f().empty()
      or request->conditions().size() != 2) {
    throw std::runtime_error("[spde.validate]: empty data");
  }
  if (request->function().x().size() != request->function().q().size() or
      request->function().x().size() != request->function().f().size()) {
    throw std::runtime_error("[spde.validate]: must be (x.size == q.size == f.size)");
  }
  if (request->kind() != spde::I) {
    throw std::runtime_error("[spde.validate]: II and III kinds are not supported yet");
  }
  if (request->method() != spde::SWEEP) {
    throw std::runtime_error("[spde.validate]: only SWEEP method supported");
  }
  if (request->conditions(0).x() >= request->function().x(0)
      or request->conditions(1).x() <= request->function().x(request->function().x().size() - 1)) {
    throw std::runtime_error("[spde.validate]: invalid initial condition data");
  }
  for (size_t i = 0; i < request->function().x_size() - 1; ++i) {
    if (request->function().x(i) >= request->function().x(i + 1)) {
      throw std::runtime_error("[spde.validate]: invalid grid of x args");
    }
  }

}

SpdeServer::~SpdeServer() = default;

