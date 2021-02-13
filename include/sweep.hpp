//
// Created by alisher on 13.02.2021.
//

#ifndef SPDE_SERVER_INCLUDE_SWEEP_HPP_
#define SPDE_SERVER_INCLUDE_SWEEP_HPP_

#include <vector>

namespace sweep {
std::vector<float> solve(const std::vector<float> &a, const std::vector<float> &b,
                         const std::vector<float> &c, const std::vector<float> &d);
}

#endif //SPDE_SERVER_INCLUDE_SWEEP_HPP_
