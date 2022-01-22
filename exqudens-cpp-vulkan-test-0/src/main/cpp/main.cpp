#include "exqudens/glfw/Application.hpp"

int main(int argc, char** argv) {
  exqudens::glfw::Application app(argc, argv);
  int result = app.run();
  return result;
}
