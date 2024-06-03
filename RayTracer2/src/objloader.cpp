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

vector<Vertex> OBJLoader::vertex_data() const
{
  vector<Vertex> data;
  float scaleFactor = 20; // the object is given in scale 1, so we scale it to 20

  // For all vertices in the model, interleave the data
  for (vec3 const& coord : d_coordinates) {
  // for (Vertex_idx const &vertex : d_vertices) {
    // vec3 const coord = d_coordinates.at(vertex.d_coord);
    // Add coordinate data
    Vertex vert;

    vert.x = coord.x * scaleFactor;
    vert.y = coord.y * scaleFactor;
    vert.z = coord.z * scaleFactor;

    data.push_back(vert);
  }

  cout << "I have " << d_vertices.size() << " vertices :)\n";
  cout << "I have " << d_coordinates.size() << " coords :)\n";
  return data; // copy elision
}

vector<Vertex> OBJLoader::unitize(string const &filename)
{
  // TODO: implement this yourself!

  parseFile(filename);
  return vertex_data();
}

// --- Private -------------------------------------------------------

void OBJLoader::parseFile(string const &filename)
{
  ifstream file(filename);
  if (file)
  {
    string line;
    while (getline(file, line))
      parseLine(line);
  }
  else
  {
    cerr << "Could not open: " << filename << " for reading!\n";
  }
}

void OBJLoader::parseLine(string const &line)
{
  if (line[0] == '#')
    return; // ignore comments

  StringList tokens = split(line, ' ', false);

  if (tokens[0] == "v")
    parseVertex(tokens);

  // Other data is also ignored
}

void OBJLoader::parseVertex(StringList const &tokens)
{
  float x, y, z;
  x = stof(tokens.at(1)); // 0 is the "v" token
  y = stof(tokens.at(2));
  z = stof(tokens.at(3));
  d_coordinates.push_back(vec3{x, y, z});
}

OBJLoader::StringList OBJLoader::split(string const &line, char splitChar, bool keepEmpty)
{
  StringList tokens;
  istringstream iss(line);
  string token;
  while (getline(iss, token, splitChar))
    if (token.size() > 0 || (token.size() == 0 && keepEmpty))
      tokens.push_back(token);

  return tokens;
}
