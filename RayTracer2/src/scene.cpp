#include "scene.h"
#include <iostream>
#include <fstream>

#include <algorithm>
#include <cmath>
#include <limits>

#include "hit.h"
#include "vertex.h"
#include "ray.h"
#include "shapes/mesh.h"
#include "objloader.h"

using namespace std;

void Scene::printMeshToFile(vector<Point> deformedObject, vector<OBJLoader::Vertex_idx> objectFaces, string const &ofname)
{
  // write deformed mesh to output file
  ofstream out(ofname);
  for (int j = 0; j != deformedObject.size(); ++j)
  {
    out << "v " << deformedObject[j].x << ' ' << deformedObject[j].y << ' ' << deformedObject[j].z << "\n";
  }

  cout << "size of d_vertices in scene.cpp: " << objectFaces.size() << "\n";
  for (int i = 0; i < objectFaces.size(); i += 3)
  {
    out << "f " << objectFaces[i].d_coord << " " << objectFaces[i + 1].d_coord << " " << objectFaces[i + 2].d_coord << "\n";
  }

  out.close();
}

pair<ObjectPtr, Hit> Scene::castRay(Ray const &ray) const
{
  // the first object in the scene is the mirror
  Hit min_hit(numeric_limits<double>::infinity(), Vector());
  ObjectPtr obj = nullptr;
  Hit hit(objects[0]->intersect(ray));
  if (hit.t < min_hit.t)
  {
    min_hit = hit;
    obj = objects[0];
  }

  // Hit hit(objects[0]->intersect(ray));
  return pair<ObjectPtr, Hit>(obj, hit);
}

// start point for ray is the eye. The ray ends in a cube vertex.
Point Scene::trace(Ray const &ray)
{
  // cout << "Ray O: " << ray.O.x << ", " << ray.O.y << ", " << ray.O.z << '\n';
  // cout << "Ray D: " << ray.D.x << ", " << ray.D.y << ", " << ray.D.z << '\n';

  pair<ObjectPtr, Hit> hit = castRay(ray);
  ObjectPtr obj = hit.first;
  Hit intersection = hit.second;
  // cout << "Hit: " << intersection.t << " and " << intersection.N.x << ", " << intersection.N.y << ", " << intersection.N.z << '\n';

  Point intersectionPoint = ray.at(intersection.t);
  // cout << "Point: " << intersectionPoint.x << ", " << intersectionPoint.y << ", " << intersectionPoint.z << '\n';
  Vector L = (ray.O - intersectionPoint).normalized();

  Vector reflectDir = - reflect(L, intersection.N).normalized();
  // Vector R = L - 2.0 * sin(PI * pow(20.0, 3)) * intersection.N.dot(L) * intersection.N;
  // cout << "Reflect: " << reflectDir.x << ", " << reflectDir.y << ", " << reflectDir.z << '\n';

  // Point V = intersectionPoint + (ray.D - intersectionPoint).length() * R;
  Point V = intersectionPoint + (ray.D - intersectionPoint).length() * reflectDir;
  // cout << "V: " << V.x << ", " << V.y << ", " << V.z << '\n';

  return V;
}

void Scene::deformObject(vector<OBJLoader::vec3> &objMesh, vector<Point> &deformedObject) {
  for (int i = 0; i < objMesh.size(); ++i)
  {
    // if (i == 1)
    // {
    //   break;
    // }
    Point objectPoint(objMesh[i].x, objMesh[i].y, objMesh[i].z);

    Ray eyeToVertex(eye, objectPoint - eye);

    Point result = trace(eyeToVertex);

    deformedObject.push_back(result);
  }
  // Point objectPoint(objMesh[0].x, objMesh[0].y, objMesh[0].z);
  // Ray eyeToVertex(eye, objectPoint - eye);
  // Point result = trace(eyeToVertex);
  // pair<ObjectPtr, Hit> hit = castRay(eyeToVertex);
  // ObjectPtr obj = hit.first;
  // Hit intersection = hit.second;
  // cout << "Normal: " << intersection.N.x << "/" << intersection.N.y << "/" << intersection.N.z << "\n";
}

void Scene::pointToScenePosition(vector<OBJLoader::vec3> &objMesh, Point posInScene) {
  // move cube mesh to the position given in the scene description
  cout << "move mesh based on object position.\n";
  for (int idx = 0; idx != objMesh.size(); ++idx)
  {
    // cout << "objectMesh.x = " << objectMesh[idx].x << "\n";
    // cout << "objectMesh.y = " << objectMesh[idx].y << "\n";
    // cout << "objectMesh.z = " << objectMesh[idx].z << "\n";
    objMesh[idx].x = objMesh[idx].x + posInScene.x;
    objMesh[idx].y = objMesh[idx].y + posInScene.y;
    objMesh[idx].z = objMesh[idx].z + posInScene.z;
  }
  cout << "object Point after translation: " << objMesh[0].x << " / " << objMesh[0].y << " / " << objMesh[0].z << "\n";
}

void Scene::translateDeformedObject(vector<Point> &object, Point newPosition) {
  for (int i = 0; i < object.size(); ++i) {
    object[i].x += newPosition.x;
    object[i].y += newPosition.y;
    object[i].z += newPosition.z;
  }
}

void Scene::render(string const &objFile, string const &ofname)
{
  OBJLoader loadObject(objFile);
  cout << "About to call unitize.\n";
  //loadObject.unitize();
  vector<OBJLoader::vec3> objectMesh = loadObject.d_coordinates;

  //test
  // vector<Point> unitizeTest;
  // for (int i = 0; i < objectMesh.size(); ++i)
  // {
  //   Point objectPoint(objectMesh[i].x, objectMesh[i].y, objectMesh[i].z);
  //   unitizeTest.push_back(objectPoint);
  // }
  // printMeshToFile(unitizeTest, loadObject.d_vertices, "../scenes/results/unitest.obj");

  Point objectOrigin = objects[1].get()->getPosition();
  cout << "Object Origin: " << objectOrigin.x << ", " << objectOrigin.y << ", " << objectOrigin.z << "\n";

  pointToScenePosition(objectMesh, objectOrigin);

  vector<Point> deformedObject;

  deformObject(objectMesh, deformedObject);

  //move so that deformed object is across the mirror from original object
  cout << "cube point position before relocation: " << deformedObject[0].x << "/" << deformedObject[0].y << "/" << deformedObject[0].z << "\n";
  //translate to origin of system
  // Point toOrigin(-deformedObject[0].x, -deformedObject[0].y, -deformedObject[0].z);
  // translateDeformedObject(deformedObject, toOrigin);
  
  // //translate to position in scene
  // Point newPosition(objectOrigin.x, objectOrigin.y, -objectOrigin.z);
  // translateDeformedObject(deformedObject, newPosition);

  printMeshToFile(deformedObject, loadObject.d_vertices, ofname);
}

// --- Misc functions ----------------------------------------------------------

// Defaults
Scene::Scene() : objects(), eye() {}

void Scene::addObject(ObjectPtr obj) { objects.push_back(obj); }

void Scene::setEye(Triple const &position) { eye = position; }

unsigned Scene::getNumObject() { return objects.size(); }

Point Scene::getEye() { return eye; }
