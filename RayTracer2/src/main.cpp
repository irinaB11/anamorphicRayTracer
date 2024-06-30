#include <iostream>
#include <string>

#include "raytracer.h"

using namespace std;

int main(int argc, char *argv[]) {
  cout << "Computer Graphics - Ray tracer\n\n";

  if (argc < 2 || argc > 3) {
    cerr << "Usage: " << argv[0] << " in-file [out-file.png]\n";
    return 1;
  }

  Raytracer raytracer;

  // read the scene
  if (!raytracer.readScene(argv[1])) {
    cerr << "Error: reading scene from " << argv[1] << " failed - no output generated.\n";
    return 1;
  }

  // determine output name
  string ofname;
  string objectFile;
  string sceneFile;
  if (argc >= 4) {
    objectFile = argv[2];
    ofname = argv[3];  // use the provided name
  } else {
    objectFile = argv[2];
    sceneFile = argv[1];
    sceneFile.erase(sceneFile.begin() + sceneFile.find_last_of('.'), sceneFile.end());
    sceneFile.erase(sceneFile.begin(), sceneFile.begin() + sceneFile.find_last_of('/'));
    //cout << "scene file: " << sceneFile << "\n";
    string filePath = "../scenes/results";
    ofname = objectFile;
    ofname.erase(ofname.begin() + ofname.find_last_of('.'), ofname.end());
    //cout << "end erased: " << ofname << "\n";
    ofname.erase(ofname.begin(), ofname.begin() + ofname.find_last_of('/'));
    //cout << "erased beginning: " << ofname << "\n";
    ofname = (filePath + ofname).append(sceneFile, 1, 4) + ".obj";
    cout << ofname << "\n";
    // ofname = ofname.append(sceneFile);
    // ofname += ".obj";
  }

  raytracer.renderToFile(ofname, objectFile);

  return 0;
}
