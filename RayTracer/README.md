# Anamorphic Ray Tracer in C++

This is a C++ framework for a ray tracer. It was originally created for the Computing Science course "Computer Graphics" taught at the University of Groningen.

This framework is inspired by and uses (some) code of the ray tracer framework of Bert Freudenberg which unfortunately is no longer available.

This framework was modified since its original use. The purpose of this version is to construct anamorphic sculptures. The input for the framework consists of a scene description and the mesh of an object. The output of the framework is another mesh. This mesh represents the deformation of the original object based on its reflection on a mirror (plane, cylinder, sphere) present in the scene.

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
After compilation you should have the `ray` executable.
This can be used like this:
```
./ray <path to .json file> <path to .obj file>
# when in the build directory:
./ray ../scenes/cylinder/scene01.json ../models/object.obj
```


NEEDS TO BE MODIGIED: Specifying an output is optional and by default an image will be created in
the same directory as the source scene file with the `.json` extension replaced
by `.png`.

## Description of the included files

### Scene files
* `scenes/*.json`: Scene files are structured in JSON. If you have never
    worked with JSON, please see [here](https://en.wikipedia.org/wiki/JSON#Data_types_and_syntax) or [here](https://www.json.org).

    Take a look at the provided example scenes for the general structure.
    You are encouraged to define your own scene files for testing your
    application.

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

* `sphere.cpp/.h (inside shapes)`: Sphere class, which is a subclass of the `Object` class. Represents a sphere in the scene. This is usually a spherical mirror.

* `cylinder.cpp/.h (inside shapes)`: Cylinder class, which is a subclass of the `Object` class. Represents a cylinder in the scene. This is usually a cylindrical mirror.

* `quad.cpp/.h (inside shapes)`: Quad class, which is a subclass of the `Object` class. Represents a 2D plane in the scene. This is usually a plane mirror.

* `mesh.cpp/.h (inside shapes)`: Mesh class, which is a subclass of the `Object` class. Represents the mesh of an object in the scene. This is the object that is deformed.

* `triple.cpp/.h`: Triple class. Represents a three-dimensional vector which is used for colors, points and vectors.
    Includes a number of useful functions and operators, see the comments in `triple.h`.
    Classes of `Color`, `Vector`, `Point` are all aliases of `Triple`.

### Supporting source files

* `lode/*`: Code for reading from and writing to PNG files,
    used by the `Image` class.
     lodepng is created by Lode Vandevenne and can be found on [github](https://github.com/lvandeve/lodepng).
     This is not used in this version of the framework.

* `json/*`: Code for parsing JSON documents.
    Created by Niels Lohmann and available under the MIT license on  [github](https://github.com/nlohmann/json).
    **Recommended:** Especially take a look at their README for more info on how to work with JSON files.