//
// Created by alisher on 13.02.2021.
//
#include <map>
#include <iostream>
#include <sweep.hpp>
namespace sweep {

std::vector<float> solve(const std::vector<float> &a, const std::vector<float> &b,
                         const std::vector<float> &c, const std::vector<float> &d) {

  if (a.empty() or b.empty() or c.empty() or d.empty()) {
    throw std::runtime_error("[sweep.solve]: vectors are shouldn't be empty");
  }
  if (a.size() != b.size() or a.size() != c.size() or a.size() != d.size()) {
    throw std::runtime_error("[sweep.solve]: must be a.size==b.size==c.size==d.size");
  }
  if (b[0] == 0) {
    throw std::runtime_error("[sweep.solve]: b[0]==0");
  }
  std::clog << "[sweep.solve]: start" << std::endl;

  std::clog << "[sweep.solve]: straight run start" << std::endl;

  std::map<size_t, float> alpha, beta, gamma;
  gamma[0] = b[0];
  alpha[0] = -c[0] / gamma[0];
  beta[0] = d[0] / gamma[0];
  const auto m = d.size() - 1;
  for (size_t i = 1; i < m; ++i) {
    gamma[i] = b[i] + a[i] * alpha[i - 1];
    alpha[i] = -c[i] / gamma[i];
    beta[i] = (d[i] - a[i] * beta[i - 1]) / gamma[i];
  }
  gamma[m] = b[m] + a[m] * alpha[m - 1];
  beta[m] = (d[m] - a[m] * beta[m - 1]) / gamma[m];
  std::clog << "[sweep.solve]: straight run done" << std::endl;

  std::clog << "[sweep.solve]: reverse run start" << std::endl;
  std::vector<float> x(d.size(), 0.0);
  x[m] = beta[m];
  for (int  i = m - 1; i >= 0; --i) {
    x[i] = alpha[i] * x[i + 1] + beta[i];
  }
  std::clog << "[sweep.solve]: reverse run done" << std::endl;

  std::clog << "[sweep.solve]: done" << std::endl;
  return x;
}

}
