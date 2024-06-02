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

pair<ObjectPtr, Hit> Scene::castRay(Ray const &ray) const {
    // the first object in the scene is the mirror
  Hit hit(objects[0]->intersect(ray));
  return pair<ObjectPtr, Hit>(objects[0], hit);
}

// start point for ray is the eye. The ray ends in a cube vertex.
Point Scene::trace(Ray const &ray) {
  pair<ObjectPtr, Hit> hit = castRay(ray);
  ObjectPtr obj = hit.first;
  Hit intersection = hit.second;

  Point intersectionPoint = ray.at(intersection.t);
  Vector L = eye - intersectionPoint;

  Vector reflectDir = reflect(L, intersection.N).normalized();

  Point V = intersectionPoint + (ray.D - intersectionPoint).length() * reflectDir;

  return V;
}

void Scene::render(string const &filePath, string const& ofname) {
    OBJLoader loadObject(filePath);
    vector<Vertex> objectMesh = loadObject.unitize(filePath);

    Point cubePosition = (*objects[1]).getPosition();
    Point cubePoint(0.0,0.0,0.0);

    //move cube mesh to the position given in the scene description
    for (int idx = 0; idx != objectMesh.size(); ++idx) {
        objectMesh[idx].x = objectMesh[idx].x + cubePosition.x;
        objectMesh[idx].y = objectMesh[idx].y + cubePosition.y;
        objectMesh[idx].z = objectMesh[idx].z + cubePosition.z;
    }

    Ray eyeToVertex( getEye(), (0.0,0.0,0.0));
    eyeToVertex.O = getEye();
    vector<Point> deformedCube;

    for (int i = 0; i != objectMesh.size(); ++i) {
        Ray eyeToVertex(getEye(),);
        eyeToVertex.D = cubePoints[i] - eyeToVertex.O;
        deformedCube[i] = trace(eyeToVertex);
    }

    //write deformed mesh to output file
    ofstream out(ofname);
    for (int j = 0; j != deformedCube.size(); ++j) {
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

