#include <spde_server.hpp>

int main(int argc, const char **argv) {
  if (argc != 2) {
    std::cout << argv[0] << " <endpoint>" << std::endl;
    return EXIT_FAILURE;
  }

  SpdeServer(argv[1]).run();
  return EXIT_SUCCESS;
}