#include "raytracer.h"
#include "triple.h"

// =============================================================================
// -- Include all your shapes here ---------------------------------------------
// =============================================================================

#include "shapes/quad.h"
#include "shapes/cylinder.h"
#include "shapes/mesh.h"
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
      cout << "Parsing sphere.\n";
    } else {
      obj = ObjectPtr(new Sphere(pos, radius));
      cout << "Parsing sphere.\n";
    }
  }
  else if (node["type"] == "quad") {
      Point v0(node["v0"]);
      Point v1(node["v1"]);
      Point v2(node["v2"]);
      Point v3(node["v3"]);
      obj = ObjectPtr(new Quad(v0, v1, v2, v3));
      cout << "Parsing quad.\n";
  } else if (node["type"] == "cylinder") {
      Point pos(node["position"]);
      double radius = node["radius"];
      Point tPoint(node["topPoint"]);
      Point bPoint(node["bottomPoint"]);
      obj = ObjectPtr(new Cylinder(pos, radius, tPoint, bPoint));
      cout << "Parsing cylinder.\n";
  } else if (node["type"] == "mesh") {
      Point pos(node["position"]);
      obj = ObjectPtr(new Mesh(pos));
      cout << "Parsing object mesh.\n"; 
  } else {
    cerr << "Unknown object type: " << node["type"] << ".\n";
  }

  // =============================================================================
  // -- End of object reading ----------------------------------------------------
  // =============================================================================

  if (!obj) return false;

  // Add object to the scene
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


void Raytracer::renderToFile(string const &ofname, string const &objFile) {
  scene.render(objFile, ofname);
  cout << "Writing object to " << ofname << "...\n";
  cout << "Done.\n";
}
