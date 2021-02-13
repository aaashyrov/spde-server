//
// Created by alisher on 13.02.2021.
//
#include <sweep.hpp>
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
    std::clog << "[spde]: solve start" << std::endl;
    validate(request);
    const auto size = request->function().x_size();
    std::vector<float> a(size, 0.0);
    std::vector<float> b(size, 0.0);
    std::vector<float> c(size, 0.0);
    std::vector<float> d(size, 0.0);

    const auto x = request->function().x();
    const auto q = request->function().q();
    const auto f = request->function().f();

    for (size_t i = 1; i < size - 1; ++i) {
      auto h2 = (x[i + 1] - x[i]) * (x[i] - x[i - 1]);
      a[i] = c[i] = -1;
      d[i] = h2 * f[i];
      b[i] = 2 + h2 * q[i];
    }
    b[0] = 1;
    c[0] = 0;
    a[size - 1] = 0;
    b[size - 1] = 1;
    d[0] = request->conditions(0).u();
    d[size - 1] = request->conditions(0).u();

    auto U = sweep::solve(a, b, c, d);
    for (const auto &u: U) {
      response->mutable_u()->Add(u);
    }

    std::clog << "[spde]: solve done" << std::endl;
    return grpc::Status(grpc::StatusCode::OK, "done");
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
  for (size_t i = 0; i < request->function().x_size() - 1; ++i) {
    if (request->function().x(i) >= request->function().x(i + 1)) {
      throw std::runtime_error("[spde.validate]: invalid grid of x args");
    }
  }
}

SpdeServer::~SpdeServer() {
  server_->Shutdown();
}