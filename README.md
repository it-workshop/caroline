# Caroline

The cake is not a lie.

Computer Vision project.

## Build instructions

Install dependencies:

 * OpenCV library. You can use any version from the repository for your system.
   On OSX you must build it yourself.

    cd /tmp
    git clone git@github.com/Itseez/opencv.git
    cd opencv
    cmake -DCMAKE_INSTALL_PREFIX=/usr/local/Celluar/opencv
    make
    make install

Clone repository:

    git clone git@github.com:it-workshop/caroline.git

Install third-party libraries.

    git submodule init
    git submodule update

Create build directory and walk into it.

    mkdir out && chdir out

Run CMake to generate build files.

    cmake ..

or, if you want to build project with ninja build system.

    cmake -G Ninja ..

Probably you will need to say to the cmake location of OpenCV files:

    cmake -DOpenCV_DIR=/usr/local/Celluar/opencv/share/OpenCV/ ..

Build project.

    make

or

    ninja

You can run unit tests.

    ./unit_tests

Remember to install git hooks before your first commit.

    ./build/install_hooks.sh

## Workflow

Each tracker issue is assigned to a collaborator. The work on each feature
must be subdivided into work packages (wp). Each wp has its own branch that
must be merged into upstream branch after review i.e. you must create a pull
request for each new work package. The branch can't be merged into the
upstream if it has compilation errors or failed unit tests on any platform.

### Names of branches

Each workpackage must be in a branch called
```wp/${issue-number}/${wp-number}```. I.e. if issue #1357 has 3 workpackages,
names of the branches would be:

 * ```wp/1357/1```;
 * ```wp/1357/2```;
 * ```wp/1357/3```.

### Commit message

The commit messages must begin from ```#${issue-number}: ```. It must be
either a single line (up to 80 characters) or multiline text with an empty
string after the first line.

## Code Conventions

The code conventions are based on the Google C++ Style guide:
http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml

Additions:

 * We use C++11.

## Tests

Use GTest testing framework. Each unit test must be built into the unit_tests
target.

Currently, the project must be tested on Linux, Windows, and Mac OS X.
We supported the following toolkits:

 * GCC 4.8;
 * CLang 3.3;
 * Microsoft Visual Studio 2013.

## Scripting

If you need to write a script you are to use bash, perl, or python.
Development tools must be saved in the ```tools``` directory, which you should
create in the ```build``` directory.

