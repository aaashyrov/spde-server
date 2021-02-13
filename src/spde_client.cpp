//
// Created by alisher on 13.02.2021.
//
#include <cmath>
#include <spde_client.hpp>

SpdeClient::SpdeClient(const std::string &endpoint)
    : stub_(spde::Spde::NewStub((grpc::CreateChannel(endpoint, grpc::InsecureChannelCredentials())))) {}

float u(float x) {
  return x * std::exp(x);
}

float f(float x) {
  return -2 * std::exp(x);
}

const std::vector<float> x = {0.0, 0.1, 0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9, 1.0};

void SpdeClient::solve() {
  grpc::ClientContext context;
  spde::Request request;
  spde::Condition condition;
  request.mutable_conditions()->Add()->set_u(u(x[0]));
  request.mutable_conditions()->Add()->set_u(u(x[10]));
  for (const auto &i:x) {
    request.mutable_function()->mutable_x()->Add(i);
  }
  for (const auto &i:x) {
    request.mutable_function()->mutable_f()->Add(f(i));
  }
  for (const auto &i:x) {
    request.mutable_function()->mutable_q()->Add(1.0);
  }
  request.set_method(spde::SWEEP);
  request.set_kind(spde::I);
  spde::Response response;
  auto status = stub_->solve(&context, request, &response);
  if (not status.ok()) {
    std::cerr << status.error_message() << std::endl;
    return;
  }
  std::cout << "1: ";
  for (const auto &i:response.u()) {
    std::cout << i << " ";
  }
  std::cout << std::endl << "2: ";

  for (const auto &i: x) {
    std::cout << u(i) << " ";
  }
  std::cout << "\n";
}
