#include "scene.h"
#include <iostream>
#include <fstream>

#include <algorithm>
#include <cmath>
#include <limits>

#include "hit.h"
#include "image.h"
#include "material.h"
#include "objloader.h"
#include "vertex.h"
#include "ray.h"
#include "shapes/cube.h"

using namespace std;

pair<ObjectPtr, Hit> Scene::castRay(Ray const &ray) const
{
  // the first object in the scene is the mirror
  Hit hit(objects[0]->intersect(ray));
  return pair<ObjectPtr, Hit>(objects[0], hit);
}

// start point for ray is the eye. The ray ends in a cube vertex.
Point Scene::trace(Ray const &ray)
{
  // cout << "Ray O: " << ray.O.x << ", " << ray.O.y << ", " << ray.O.z << '\n';
  // cout << "Ray D: " << ray.D.x << ", " << ray.D.y << ", " << ray.D.z << '\n';

  pair<ObjectPtr, Hit> hit = castRay(ray);
  ObjectPtr obj = hit.first;
  Hit intersection = hit.second;
  //cout << "Hit: " << intersection.t << " and " << intersection.N.x << ", " << intersection.N.y << ", " << intersection.N.z << '\n';

  Point intersectionPoint = ray.at(intersection.t);
  //cout << "Point: " << intersectionPoint.x << ", " << intersectionPoint.y << ", " << intersectionPoint.z << '\n';
  Vector L = eye - intersectionPoint;

  Vector reflectDir = reflect(L, intersection.N).normalized();
  //cout << "Reflect: " << reflectDir.x << ", " << reflectDir.y << ", " << reflectDir.z << '\n';

  Point V = intersectionPoint + (ray.D - intersectionPoint).length() * reflectDir;
  //cout << "V: " << V.x << ", " << V.y << ", " << V.z << '\n';

  return V;
}

void Scene::render(string const &filePath, string const &ofname)
{
  OBJLoader loadObject(filePath);
  cout << "About to call unitize.\n";
  vector<Vertex> objectMesh = loadObject.unitize(filePath);

  Point cubePosition(150.0, 0.0, 0.0); // figure out how to get position from pointer to obj
  Point cubePoint(0.0, 0.0, 0.0);

  // move cube mesh to the position given in the scene description
  cout << "move mesh based on object position.\n";
  for (int idx = 0; idx != objectMesh.size(); ++idx)
  {
    // cout << "objectMesh.x = " << objectMesh[idx].x << "\n";
    // cout << "objectMesh.y = " << objectMesh[idx].y << "\n";
    // cout << "objectMesh.z = " << objectMesh[idx].z << "\n";
    objectMesh[idx].x = objectMesh[idx].x + cubePosition.x;
    objectMesh[idx].y = objectMesh[idx].y + cubePosition.y;
    objectMesh[idx].z = objectMesh[idx].z + cubePosition.z;
  }

  Vector direction(0.0, 0.0, 0.0);
  Ray eyeToVertex(eye, direction);
  vector<Point> deformedCube;

  for (int i = 0; i != objectMesh.size(); ++i)
  {
    // if (i == 1)
    // {
    //   break;
    // }

    cubePoint.x = objectMesh[i].x;
    cubePoint.y = objectMesh[i].y;
    cubePoint.z = objectMesh[i].z;
    eyeToVertex.D = cubePoint - eyeToVertex.O;

    Point result = trace(eyeToVertex);
    deformedCube.push_back(result);
  }

  // write deformed mesh to output file
  ofstream out(ofname);
  for (int j = 0; j != deformedCube.size(); ++j)
  {
    out << "v " << deformedCube[j].x << ' ' << deformedCube[j].y << ' ' << deformedCube[j].z << "\n";
  }
  out.close();
}

// --- Misc functions ----------------------------------------------------------

// Defaults
Scene::Scene() : objects(), eye() {}

void Scene::addObject(ObjectPtr obj) { objects.push_back(obj); }

void Scene::setEye(Triple const &position) { eye = position; }

unsigned Scene::getNumObject() { return objects.size(); }

Point Scene::getEye() { return eye; }
