#include <iostream>
#include <string>

#include "raytracer.h"

using namespace std;

int main(int argc, char *argv[])
{
  cout << "Anamorphic Ray tracer\n\n";

  if (argc < 2 || argc > 3)
  {
    cerr << "Usage: " << argv[0] << " in-file [out-file.png]\n";
    return 1;
  }

  // Initilise ray tracer.
  Raytracer raytracer;

  // Read the scene.
  if (!raytracer.readScene(argv[1]))
  {
    cerr << "Error: reading scene from " << argv[1] << " failed - no output generated.\n";
    return 1;
  }

  // Determine output name.
  string ofname;
  string objectFile;
  string sceneFile;
  objectFile = argv[2];
  sceneFile = argv[1];
  sceneFile.erase(sceneFile.begin() + sceneFile.find_last_of('.'), sceneFile.end());
  sceneFile.erase(sceneFile.begin() + sceneFile.find_first_of('/'));
  sceneFile.erase(sceneFile.begin(), sceneFile.begin() + sceneFile.find_first_of('/'));
  string filePath = "../results";
  ofname = objectFile;
  ofname.erase(ofname.begin() + ofname.find_last_of('.'), ofname.end());
  ofname.erase(ofname.begin(), ofname.begin() + ofname.find_last_of('/'));
  ofname.erase(ofname.begin() + ofname.find_first_of('/'));
  ofname = filePath + sceneFile + ofname + ".obj";

  // Write points' coordinates and faces of new object to file "ofname".
  raytracer.renderToFile(ofname, objectFile);

  return 0;
}
