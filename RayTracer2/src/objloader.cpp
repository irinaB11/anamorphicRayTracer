#include "objloader.h"

// Pro C++ Tip: here you can specify other includes you may need
// such as <iostream>

#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;

// ===================================================================
// -- Constructors and destructor ------------------------------------
// ===================================================================

// --- Public --------------------------------------------------------

OBJLoader::OBJLoader(string const &filename) : d_hasTexCoords(false) { parseFile(filename); }

// ===================================================================
// -- Member functions -----------------------------------------------
// ===================================================================

// --- Public --------------------------------------------------------

vector<Vertex> OBJLoader::vertex_data() const {
  vector<Vertex> data;
  float scaleFactor = 20; //the object is given in scale 1, so we scale it to 20

  // For all vertices in the model, interleave the data
  for (Vertex_idx const &vertex : d_vertices) {
    // Add coordinate data
    Vertex vert;

    vec3 const coord = d_coordinates.at(vertex.d_coord);
    vert.x = coord.x * scaleFactor;
    vert.y = coord.y * scaleFactor;
    vert.z = coord.z * scaleFactor;

    data.push_back(vert);
  }

  return data;  // copy elision
}

vector<Vertex> OBJLoader::unitize(string const& filename) {
  // TODO: implement this yourself!

  // This is a very handy function for importing models
  // which you may reuse in other projects.
  // You may have noticed you can use arbitrary sizes for your
  // models. You may find that modelers do not always use the
  // same size for models. Therefore it might be preferable to
  // scale the object to fit inside the unit cube so you can easily
  // set the right scale of your model in OpenGL. Aditionally,
  // the model does not have to be centered around the origin
  // (0, 0, 0) which may cause troubles when translating
  // This function should fix that!

  // A common approach looks like this:

  // Determine min / max and offset in each dimension
  // Determine by which factor to scale (largest difference
  //  in min / max in a dimension (Important! Scale uniformaly in
  //  all dimensions!)
  // Loop over all coordinate data and scale the coordinates
  //  and apply the translate/scaling

    parseFile(filename);
    return vertex_data();

 // cerr << "unitize() is not implemented!\n";
}

// --- Private -------------------------------------------------------

void OBJLoader::parseFile(string const &filename) {
  ifstream file(filename);
  if (file) {
    string line;
    while (getline(file, line)) parseLine(line);

  } else {
    cerr << "Could not open: " << filename << " for reading!\n";
  }
}

void OBJLoader::parseLine(string const &line) {
  if (line[0] == '#') return;  // ignore comments

  StringList tokens = split(line, ' ', false);

  if (tokens[0] == "v")
    parseVertex(tokens);

  // Other data is also ignored
}

void OBJLoader::parseVertex(StringList const &tokens) {
  float x, y, z;
  x = stof(tokens.at(1));  // 0 is the "v" token
  y = stof(tokens.at(2));
  z = stof(tokens.at(3));
  d_coordinates.push_back(vec3{x, y, z});
}

OBJLoader::StringList OBJLoader::split(string const &line, char splitChar, bool keepEmpty) {
  StringList tokens;
  istringstream iss(line);
  string token;
  while (getline(iss, token, splitChar))
    if (token.size() > 0 || (token.size() == 0 && keepEmpty)) tokens.push_back(token);

  return tokens;
}
