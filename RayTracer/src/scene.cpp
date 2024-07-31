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

/*
 * Writes the vertices and the faces of the deformed mesh to the given file.
 */
void Scene::printMeshToFile(vector<Point> deformedObject, vector<OBJLoader::Vertex_idx> objectFaces, string const &ofname)
{
  // write deformed mesh to output file
  ofstream out(ofname);
  for (int j = 0; j != deformedObject.size(); ++j)
  {
    out << "v " << deformedObject[j].x << ' ' << deformedObject[j].y << ' ' << deformedObject[j].z << "\n";
  }

  for (int i = 0; i < objectFaces.size(); i += 3)
  {
    out << "f " << objectFaces[i].d_coord << " " << objectFaces[i + 1].d_coord << " " << objectFaces[i + 2].d_coord << "\n";
  }

  out.close();
}

/*
 * Calculates the intersection point between a ray and the mirror. 
 */
pair<ObjectPtr, Hit> Scene::castRay(Ray const &ray) const
{
  // the first object in the scene object pointer is the mirror
  Hit min_hit(numeric_limits<double>::infinity(), Vector());
  ObjectPtr obj = nullptr;
  Hit hit(objects[0]->intersect(ray));
  if (hit.t < min_hit.t)
  {
    min_hit = hit;
    obj = objects[0];
  }
  return pair<ObjectPtr, Hit>(obj, hit);
}

/*
 * Calculates the new position of the vertex along the reflection ray.
 */
Point Scene::trace(Ray const &ray)
{
  pair<ObjectPtr, Hit> hit = castRay(ray);
  ObjectPtr obj = hit.first;
  Hit intersection = hit.second;

  Point intersectionPoint = ray.at(intersection.t);

  Vector L = (ray.O - intersectionPoint).normalized();

  Vector reflectDir = reflect(L, intersection.N).normalized();

  Point V = intersectionPoint + (ray.D - intersectionPoint).length() * reflectDir;

  return V;
}

/* 
 * For every vertex in the mesh, the ray starting from the camera going to the vertex is
 * calculated. A fucntion that calculates the deformed vertex is then called.
 */
void Scene::deformObject(vector<OBJLoader::vec3> &objMesh, vector<Point> &deformedObject)
{
  for (int i = 0; i < objMesh.size(); ++i)
  {
    Point objectPoint(objMesh[i].x, objMesh[i].y, objMesh[i].z);

    Ray eyeToVertex(eye, objectPoint - eye);

    Point result = trace(eyeToVertex);

    deformedObject.push_back(result);
  }
}

/*
 * Move the mesh to the position given in the scene description.
 */ 
void Scene::moveToScenePosition(vector<OBJLoader::vec3> &objMesh, Point posInScene)
{
  for (int idx = 0; idx != objMesh.size(); ++idx)
  {
    objMesh[idx].x = objMesh[idx].x + posInScene.x;
    objMesh[idx].y = objMesh[idx].y + posInScene.y;
    objMesh[idx].z = objMesh[idx].z + posInScene.z;
  }
}

/* 
 * Helper function to check if the original mesh shrinked correctly.
 */
void Scene::checkUnitizedMesh(OBJLoader &object, vector<OBJLoader::vec3> &mesh)
{
  // Prints the unitized mesh of the object to "../results/unitizeObject.obj".
  // This way it is possible to check if the object shrinked uniformly.
  vector<Point> unitizeTest;
  for (int i = 0; i < mesh.size(); ++i)
  {
    Point objectPoint(mesh[i].x, mesh[i].y, mesh[i].z);
    unitizeTest.push_back(objectPoint);
  }
  printMeshToFile(unitizeTest, object.d_vertices, "../results/unitizedObject.obj");
}

/*
 * Loads the mesh of the obejct that will be deformed. Then calls functions to shrink it,
 * if it is necessary. Then it moves the points of the mesh such that the object is 
 * positioned in the scene at the position given in the file description. After that it
 * calls the function to deform the mesh and prints it to the file pased to the function.
 */
void Scene::render(string const &objFile, string const &ofname)
{
  OBJLoader loadObject(objFile);

  // Call when the ray that goes from the camera to a vertex on the object does not intersect the
  // mirror. This will result in "nan" values in the file of the deformed object.
  loadObject.unitize();

  vector<OBJLoader::vec3> objectMesh = loadObject.d_coordinates;

  // Call only if "loadObject.unitize();" is called above.
  // checkUnitizedMesh(loadObject, objectMesh);

  Point objectOrigin = objects[1].get()->getPosition();

  moveToScenePosition(objectMesh, objectOrigin);

  vector<Point> deformedObject;

  deformObject(objectMesh, deformedObject);

  printMeshToFile(deformedObject, loadObject.d_vertices, ofname);
}

// --- Misc functions ----------------------------------------------------------

// Defaults
Scene::Scene() : objects(), eye() {}

void Scene::addObject(ObjectPtr obj) { objects.push_back(obj); }

void Scene::setEye(Triple const &position) { eye = position; }

unsigned Scene::getNumObject() { return objects.size(); }

Point Scene::getEye() { return eye; }
