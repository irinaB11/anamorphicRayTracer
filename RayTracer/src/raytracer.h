#ifndef RAYTRACER_H_
#define RAYTRACER_H_

#include <string>

#include "scene.h"

#include "json/json_fwd.h"

class Raytracer {
  Scene scene;

 public:
  bool readScene(std::string const &ifname);
  void renderToFile(std::string const &ofname, std::string const &objFile);

 private:
  bool parseObjectNode(nlohmann::json const &node);
};

#endif
