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

  //For all vertices in the model, interleave the data
  std::cout << "I have " << d_vertices.size() << " vertices before loop:)\n";
  for (Vertex_idx const &vertex : d_vertices) {
    // Add coordinate data
    Vertex vert;

    vec3 const coord = d_coordinates.at(vertex.d_coord);
    vert.x = coord.x;
    vert.y = coord.y;
    vert.z = coord.z;

    // Add normal data
    vec3 const norm = d_normals.at(vertex.d_norm);
    vert.nx = norm.x;
    vert.ny = norm.y;
    vert.nz = norm.z;

    // Add texture data (if available)
    if (d_hasTexCoords) {
      vec2 const tex = d_texCoords.at(vertex.d_tex);
      vert.u = tex.u;  // u coordinate
      vert.v = tex.v;  // v coordinate
    } else {
      vert.u = 0;
      vert.v = 0;
    }
    data.push_back(vert);
  }

  std::cout << "I have " << d_vertices.size() << " vertices :)\n";
  std::cout << "I have " << d_normals.size() << " normals. \n";
  std::cout << "I have " << d_texCoords.size() << " texture coordinates. \n";
  std::cout << "I have " << d_coordinates.size() << " coords :)\n";
  return data; // copy elision
}

unsigned OBJLoader::numTriangles() const { return d_vertices.size() / 3U; }

bool OBJLoader::hasTexCoords() const { return d_hasTexCoords; }

// void translateMesh(Point translation) {

// }

void OBJLoader::unitize()
{
  float minX = 0;
  float maxX = 0;
  float minY = 0;
  float maxY = 0;
  float minZ = 0;
  float maxZ = 0;

  for (int i = 0; i < d_coordinates.size(); ++i) {
    minX = min(minX, d_coordinates[i].x);
    maxX = max(maxX, d_coordinates[i].x);
    minY = min(minX, d_coordinates[i].y);
    maxY = max(maxX, d_coordinates[i].y);
    minZ = min(minX, d_coordinates[i].z);
    maxZ = max(maxX, d_coordinates[i].z);
  }

  // cout << "minX: " << minX << "\n";
  // cout << "maxX: " << maxX << "\n";
  // cout << "minY: " << minY << "\n";
  // cout << "maxY: " << maxY << "\n";
  // cout << "minZ: " << minZ << "\n";
  // cout << "maxZ: " << maxZ << "\n";

  float offsetX = abs(maxX - minX);
  float offsetY = abs(maxY - minY);
  float offsetZ = abs(maxZ - minZ);

  // cout << "offsetX: " << offsetX << "\n";
  // cout << "offsetY: " << offsetY << "\n";
  // cout << "offsetZ: " << offsetZ << "\n";

  if (offsetX != 0 && offsetY != 0 && offsetZ != 0) {
    for (int idx = 0; idx < d_coordinates.size(); idx++)
    {
      d_coordinates[idx].x /= offsetX;
      d_coordinates[idx].y /= offsetY;
      d_coordinates[idx].z /= offsetZ;
    }
    cout << "Object mesh has been scaled. \n";
  }
  // cout << "d_coordinates[0]: " << d_coordinates[0].x << "/" << d_coordinates[0].y << "/" << d_coordinates[0].z << "\n";
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
  if (line[0] == '#') return;  // ignore comments

  StringList tokens = split(line, ' ', false);

  if (tokens[0] == "v")
    parseVertex(tokens);
  else if (tokens[0] == "vn")
    parseNormal(tokens);
  else if (tokens[0] == "vt")
    parseTexCoord(tokens);
  else if (tokens[0] == "f")
    parseFace(tokens);
}

void OBJLoader::parseVertex(StringList const &tokens)
{
  float x, y, z;
  x = stof(tokens.at(1)); // 0 is the "v" token
  y = stof(tokens.at(2));
  z = stof(tokens.at(3));
  d_coordinates.push_back(vec3{x, y, z});
  //std::cout << "vertex: " << x << " " << y << " " << z << "\n";
}

void OBJLoader::parseNormal(StringList const &tokens) {
  float x, y, z;
  x = stof(tokens.at(1));  // 0 is the "vn" token
  y = stof(tokens.at(2));
  z = stof(tokens.at(3));
  d_normals.push_back(vec3{x, y, z});
  //std::cout << "normals: " << x << " " << y << " " << z << "\n";
}

void OBJLoader::parseTexCoord(StringList const &tokens) {
  d_hasTexCoords = true;  // Texture data will be read

  float u, v;
  u = stof(tokens.at(1));  // 0 is the "vt" token
  v = stof(tokens.at(2));
  d_texCoords.push_back(vec2{u, v});
}


void OBJLoader::parseFace(StringList const &tokens) {
  // skip the first token ("f")
  for (size_t idx = 1; idx < tokens.size(); ++idx) {
    // format is:
    // <vertex idx + 1>/<texture idx +1>/<normal idx + 1>
    // Wavefront .obj files start counting from 1 (yuck)

    StringList elements = split(tokens.at(idx), '/');
    Vertex_idx vertex{};  // initialize to zeros on all fields

    vertex.d_coord = stoul(elements.at(0)); //used to be -1U
    //int x = stoul(elements.at(0));

    //int y;
    if (d_hasTexCoords) {
      vertex.d_tex = stoul(elements.at(1)); //used to be -1U
      //y = stoul(elements.at(1));
    } else {
      vertex.d_tex = 0U;  // ignored
      //y = 0;
    }

    vertex.d_norm = stoul(elements.at(2)); //used to be -1U
    //int z = stoul(elements.at(2));

    //Face faceVertex(x, y, z);

    d_vertices.push_back(vertex);
    //faces.push_back(faceVertex);

    // std::cout << "coord: " << vertex.d_coord << "\n";
    // std::cout << "texture: " << vertex.d_tex << "\n";
    // std::cout << "norm: " << vertex.d_norm << "\n";
  }
  //std::cout << "I have " << d_vertices.size() << " vertices in parseFace.\n";
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
