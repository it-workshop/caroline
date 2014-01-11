# Caroline

The cake is not a lie.

Computer Vision project.

## Build instructions

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

Build project.

    make

or

    ninja

You can run unit tests.

    ./unit_tests

## Workflow

Each tracker issue is assigned to a collaborator. The work on each feature must be
subdivided into work packages (wp). Each wp has its own branch that must be merged
into upstream branch after review i.e. you must create a pull request for each new
work package. The branch can't be merged into the upstream if it has compilation
errors or failed unit tests on any platform.

### Names of branches

Each workpackage must be in a branch called ``wp/${issue-number}/${wp-number}``.
i.e. if issue#1357 has 3 workpackages, names of the branches would be:
``wp/1357/1``,
``wp/1357/2``,
``wp/1357/3``.

### Commit message

The commit messages must begin from ``#${issue-number}: ``. It must be
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
 * CLang 3.3.

## Scripting

If you need to write a script you are to use bash, perl, or python.
Development tools must be saved in the ``tools`` directory, which you should
create in the ``build`` directory.
