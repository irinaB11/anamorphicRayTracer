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
  if (argc >= 4) {
    objectFile = argv[2];
    ofname = argv[3];  // use the provided name
  } else {
    objectFile = argv[2];
    string filePath = "../scenes/results";
    ofname = objectFile;
    ofname.erase(ofname.begin(), ofname.begin() + ofname.find_last_of('/'));
    cout << "erased beginning: " << ofname << "\n";
    ofname = filePath.append(ofname);
  }

  raytracer.renderToFile(ofname, objectFile);

  return 0;
}
