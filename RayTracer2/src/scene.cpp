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

  //// No hit? Return background color.
  //if (!obj) return Color(0.0, 0.0, 0.0);

  //Material const &material = obj->material;
  //Point hit = ray.at(min_hit.t);
  //Vector V = -ray.D;

  //// Pre-condition: For closed objects, N points outwards.
  //Vector N = min_hit.N;

  //// The shading normal always points in the direction of the view,
  //// as required by the Phong illumination model.
  //Vector shadingN;
  //if (N.dot(V) >= 0.0)
  //  shadingN = N;
  //else
  //  shadingN = -N;

  //// Set the color of the material
  //Color matColor = material.color;

  //// If the object has a texture, use that one instead
  //if (material.hasTexture) {
  //  // Compute local UV coordinates of the hit within the texture
  //  Vector uv = obj->toUV(hit);
  //  double u  = uv.x;
  //  double v  = uv.y;

  //  // Material color will be read from the texture in local coordinates
  //  matColor = material.texture.colorAt(u, 1.0 - v);
  //}

  //// Add ambient once, regardless of the number of lights.
  //Color color = material.ka * matColor;

  //// Add diffuse and specular components.
  //for (auto const &light : lights) {
  //  Vector L = (light->position - hit).normalized();

  //  // Only add diffuse/specular components if shadows are rendered
  //  // and no intersection occurs with any objects in the scene
  //  if (renderShadows && intersectsAny(hit, light))
  //    continue;

  //  // Add diffuse.
  //  double dotNormal = shadingN.dot(L);
  //  double diffuse = std::max(dotNormal, 0.0);
  //  color += diffuse * material.kd * light->color * matColor;

  //  // Add specular.
  //  if (dotNormal > 0) {
  //    Vector reflectDir = reflect(-L, shadingN);
  //    double specAngle = max(reflectDir.dot(V), 0.0);
  //    double specular = pow(specAngle, material.n);

  //    color += specular * material.ks * light->color;
  //  }
  //}

  //// Compute reflection components
  //Vector reflectDir = reflect(ray.D, shadingN).normalized();
  //Ray reflectRay(hit + epsilon * shadingN, reflectDir);

  //// The object is transparent, and thus refracts and reflects light.
  //// Use Schlick's approximation to determine the ratio between the two.
  //if (depth > 0.0 && material.isTransparent) {
  //  double n_i = 1.0;
  //  double n_t = material.nt; 

  //  if (N.dot(V) < 0)
  //    std::swap(n_i, n_t);

  //  // Use Schlick's approximation to determine the ratio of
  //  // reflected (k_r) and refracted (k_t) color
  //  double k_r0 = pow((-n_i + n_t) / (n_i + n_t), 2);
  //  double k_r  = k_r0 + (n_i - k_r0) * pow(1 - shadingN.dot(V), 5);
  //  double k_t  = 1 - k_r;

  //  // Compute refraction components
  //  Vector refractDir = refract(ray.D, shadingN, n_i, n_t).normalized();
  //  Ray refractRay(hit + epsilon * -shadingN, refractDir);

  //  // Add reflection/refraction.
  //  color += k_r * trace(reflectRay, depth - 1);
  //  color += k_t * trace(refractRay, depth - 1);
  //}

  //// The object is not transparent, but opaque.
  //else if (depth > 0.0 && material.ks > 0.0)
  //  color += material.ks * trace(reflectRay, depth - 1);

  //return color;
}

void Scene::render(string const &filePath, string const& ofname) {
    OBJLoader loadObject;
    vector<Vertex> objectMesh = loadObject.unitize(filePath);

    Object cube = &(objects.rbegin());
    Point cubePosition = cube.position;
    vector<Point> cubePoints;

    //move cube mesh to the position given in the scene description
    for (int idx = 0; idx != objectMesh.size(); ++idx) {
        cubePoints[idx].x = objectMesh[idx].x + cubePosition.x;
        cubePoints[idx].y = objectMesh[idx].y + cubePosition.y;
        cubePoints[idx].z = objectMesh[idx].z + cubePosition.z;
    }

    Ray eyeToVertex;
    eyeToVertex.O = getEye();
    vector<Point> deformedCube;

    for (int i = 0; i != cubePoints.size(); ++i) {
        eyeToVertex.D = cubePoints[i] - eyeToVertex.O;
        deformedCube[i] = trace(eyeToVertex);
    }

    //write deformed mesh to output file
    ofstream out(ofname);
    for (int j = 0; j != deformedCube.size(); ++j) {
        out << "v " << deformedCube[j].x << ' ' << deformedCube[j].y << ' ' << deformedCube[j].z << "\n";
    }
    out.close();

  //unsigned w = img.width();
  //unsigned h = img.height();

  //double dist = 1.0 / supersamplingFactor;

  //for (unsigned y = 0; y < h; ++y)
  //  for (unsigned x = 0; x < w; ++x) {
  //    // Use an average color of all the samples from super sampling
  //    Color col(0, 0, 0);

  //    // Use super sampling to divide each pixel further
  //    for (unsigned ny = 0; ny < supersamplingFactor; ++ny) {
  //      for (unsigned nx = 0; nx < supersamplingFactor; ++nx) {
  //        // Get the divided pixel
  //        double pX = x + (nx * dist) + 0.5 * dist;
  //        double pY = h - 1 - y + (ny * dist) + 0.5 * dist;
  //        Point pixel(pX, pY, 0);

  //        // Add on each sampled color to the result
  //        Ray ray(eye, (pixel - eye).normalized());
  //        col += trace(ray, recursionDepth).clamp();
  //      }
  //    }

  //    // Get the average color by dividing over the amount of samples
  //    col /= std::pow(supersamplingFactor, 2);

  //    col.clamp();
  //    img(x, y) = col;
  //  }
}

//bool Scene::intersectsAny(Point const& point, LightPtr const& light) const {
//  // Determine the direction from the point to the light
//  Vector dir = (light->position - point).normalized();
//
//  // Calculate the ray by starting the intersection checking from
//  // point + eps * dir to avoid numerical imprecision causing the ray
//  // to hit the surface the point is on
//  Ray ray(point + epsilon * dir, dir);
//
//  // Go through the objects one by one
//  for (auto const &object : objects) {
//    // If the object is invalid, it cannot be intersected
//    if (!object)
//      continue;
//
//    // Determine the hit occurs between the ray and the object
//    Hit hit = object->intersect(ray);
//
//    // Objects can only intersect when the time is set and it is non-negative
//    if (isnan(hit.t) || hit.t < 0)
//      continue;
//
//    // If the intersection occurs before hitting the light, it is valid
//    Point objPosition = ray.at(hit.t);
//    double objDist    = (objPosition - ray.O).length();
//    double lightDist  = (light->position - ray.O).length();
//
//    if (objDist < lightDist)
//      return true;
//  }
//
//  return false;
//}

// --- Misc functions ----------------------------------------------------------

// Defaults
Scene::Scene() : objects(), lights(), eye(), renderShadows(false), recursionDepth(0), supersamplingFactor(1) {}

//void Scene::addObject(ObjectPtr obj) { objects.push_back(obj); }
//
//void Scene::addLight(Light const &light) { lights.push_back(LightPtr(new Light(light))); }

void Scene::setEye(Triple const &position) { eye = position; }

unsigned Scene::getNumObject() { return objects.size(); }

//unsigned Scene::getNumLights() { return lights.size(); }

unsigned Scene::getEye() { return eye; }

/*void Scene::setRenderShadows(bool shadows) { renderShadows = shadows; }

void Scene::setRecursionDepth(unsigned depth) { recursionDepth = depth; }

void Scene::setSuperSample(unsigned factor) { supersamplingFactor = factor; */}
