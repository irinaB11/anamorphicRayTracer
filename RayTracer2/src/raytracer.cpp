#include "raytracer.h"

#include "image.h"
#include "light.h"
#include "material.h"
#include "triple.h"

// =============================================================================
// -- Include all your shapes here ---------------------------------------------
// =============================================================================

#include "shapes/quad.h"
#include "shapes/cylinder.h"
#include "shapes/cube.h"
#include "shapes/sphere.h"

// =============================================================================
// -- End of shape includes ----------------------------------------------------
// =============================================================================

#include <exception>
#include <fstream>
#include <iostream>

#include "json/json.h"

using namespace std;  // no std:: required
using json = nlohmann::json;

bool Raytracer::parseObjectNode(json const &node) {
  ObjectPtr obj = nullptr;

  // =============================================================================
  // -- Determine type and parse object parametrers ------------------------------
  // =============================================================================

  if (node["type"] == "sphere") {
    Point pos(node["position"]);
    double radius = node["radius"];
    if (node.count("rotation")) {
      // Create sphere with rotation
      Vector rotation(node["rotation"]);
      double angle = node["angle"];
      obj = ObjectPtr(new Sphere(pos, radius, rotation, angle));
    } else {
      obj = ObjectPtr(new Sphere(pos, radius));
    }
  }
  else if (node["type"] == "quad") {
      Point v0(node["v0"]);
      Point v1(node["v1"]);
      Point v2(node["v2"]);
      Point v3(node["v3"]);
      obj = ObjectPtr(new Quad(v0, v1, v2, v3));
  } else if (node["type"] == "cylinder") {
      Point pos(node["position"]);
      double radius = node["radius"];
      double height = node["height"];
      obj = ObjectPtr(new Cylinder(pos, radius, height));
      cout << "Parsing cylinder.\n";
  } else if (node["type"] == "highVertexCube") {
      Point pos(node["position"]);
      obj = ObjectPtr(new Cube(pos));
      cout << "Parsing (highVertex)cube.\n";
      //put in object loader to read cube data 
  } else {
    cerr << "Unknown object type: " << node["type"] << ".\n";
  }

  // =============================================================================
  // -- End of object reading ----------------------------------------------------
  // =============================================================================

  if (!obj) return false;

  // Parse material and add object to the scene
  //obj->material = parseMaterialNode(node["material"]);
  scene.addObject(obj);
  return true;
}

bool Raytracer::readScene(string const &ifname) try {
  // Read and parse input json file
  ifstream infile(ifname);
  if (!infile) throw runtime_error("Could not open input file for reading.");
  json jsonscene;
  infile >> jsonscene;

  // =============================================================================
  // -- Read your scene data in this section -------------------------------------
  // =============================================================================

  Point eye(jsonscene["Eye"]);
  scene.setEye(eye);

  unsigned objCount = 0;
  for (auto const &objectNode : jsonscene["Objects"])
    if (parseObjectNode(objectNode)) ++objCount;

  cout << "Parsed " << objCount << " objects.\n";

  // =============================================================================
  // -- End of scene data reading ------------------------------------------------
  // =============================================================================

  return true;
} catch (exception const &ex) {
  cerr << ex.what() << '\n';
  return false;
}

void Raytracer::renderToFile(string const &ofname) {
  // TODO: the size may be a settings in your file
  scene.render("../models/goatDeformed.obj", ofname);
  cout << "Writing object to " << ofname << "...\n";
  cout << "Done.\n";
}
