# Anamorphic Ray Tracer in C++

This is a C++ framework for a ray tracer. It was originally created for the Computing Science course "Computer Graphics" taught at the University of Groningen.

This framework is inspired by and uses (some) code of the ray tracer framework of Bert Freudenberg which unfortunately is no longer available.

This framework was modified since its original use. The purpose of this version is to construct anamorphic sculptures. The input for the framework consists of a scene description and the mesh of an object. The output of the framework is another mesh. This mesh represents the deformation of the original object based on its reflection on a mirror (plane, cylinder, sphere) present in the scene.

The resulting objects of this program are used in [VRT](https://github.com/irinaB11/Virtual-Ray-Tracer-Anamorphic-Sculptures/tree/VRT-RTX-Anamorphic-Sculptures) to build a scene that explains the process of creating anamorphic sculptures. It is also possible to check the reflections of these objects in real time in VRT.

## Compiling the code

We provided a [CMakeLists.txt](CMakeLists.txt) file to be used with `CMake`.
This can be used in the following way (on the command line/shell):

```
# cd into the main directory of the framework
mkdir build   # create a new build directory
cd build      # cd into the directory
cmake ..      # creates a Makefile
# Compiling the code can then be done with:
make
# or
make -j4      # replacing 4 with the number of cores of your computer
```

A simple build script has also been provided in `build.sh`.

**Note!** After adding new `.cpp` files, `cmake ..` needs to be called again or you might get linker errors.

## Running the Ray tracer
After compilation, you should have the `ray` executable.
This can be used like this:
```
./ray <path to .json file> <path to .obj file>
# When in the build directory:
./ray ../scenes/cylinder/scene01.json ../models/object.obj
```
The output file does not need to be specified. It is determined by the type of scene and object given as input. In the scenes folder are three other folders. Each folder is named after the type of mirror present in the scene description. In the folders named after the mirror type, there are two different .json files. They describe the position of the camera, the mirror, and the object we want to deform in the scene. There are two types of scenes in each folder. 1.json is a scene description based on the scene in VRT. 2.json is a scene description based on a scene model made in Blender. These models can be found in the folder SceneModels found in this repository.

The output file is built by adding the scene number (1 or 2) in front of the model's name. It is saved in the folder results in the folder named after the type of mirror used. For example, given ../scenes/cylinder/1.json and ../models/cube.obj, the output file will be ../results/cylinder/1cube.obj.

## Description of the included files

### Scene files
* `scenes/*.json`: Scene files are structured in JSON. If you have never
    worked with JSON, please see [here](https://en.wikipedia.org/wiki/JSON#Data_types_and_syntax) or [here](https://www.json.org).

    Take a look at the provided example scenes for the general structure.
    You are encouraged to define your own scene files for testing your
    application. Creating scenes in Blender and using them as references is a good way to write new scene descriptions.

### The ray tracer source files

* `main.cpp`: Contains main(), starting point. Responsible for parsing
    command-line arguments.

* `raytracer.cpp/.h`: Ray tracer class. Responsible for reading the scene description, starting the ray tracer and writing the result to an .obj file.

* `scene.cpp/.h`: Scene class. Contains code for the actual object deformation using ray tracing.

* `ray.h`: Ray class. POD class. Ray from an origin point in a direction.

* `hit.h`: Hit class. POD class. Intersection between an `Ray` and an `Object`.

* `object.h`: virtual `Object` class. Represents an object in the scene.
    All your shapes should derive from this class.

* `shapes (directory/folder)`: Folder containing all your shapes.

* `sphere.cpp/.h (inside shapes)`: Sphere class, a subclass of the `Object` class. Represents a sphere in the scene. This is usually a spherical mirror.

* `cylinder.cpp/.h (inside shapes)`: Cylinder class, a subclass of the `Object` class. Represents a cylinder in the scene. This is usually a cylindrical mirror.

* `quad.cpp/.h (inside shapes)`: Quad class, a subclass of the `Object` class. Represents a 2D plane in the scene. This is usually a plane mirror.

* `mesh.cpp/.h (inside shapes)`: Mesh class, a subclass of the `Object` class. Represents the mesh of an object in the scene. This is the object that is deformed.

* `triple.cpp/.h`: Triple class. Represents a three-dimensional vector which is used for colors, points and vectors.
    Includes several useful functions and operators, see the comments in `triple.h`.
    Classes of `Color`, `Vector`, `Point` are all aliases of `Triple`.

### Supporting source files

* `lode/*`: Code for reading from and writing to PNG files,
    used by the `Image` class.
     lodepng is created by Lode Vandevenne and can be found on [github](https://github.com/lvandeve/lodepng).
     This is not used in this version of the framework.

* `json/*`: Code for parsing JSON documents.
    Created by Niels Lohmann and available under the MIT license on  [github](https://github.com/nlohmann/json).
    **Recommended:** Especially take a look at their README for more info on how to work with JSON files.
