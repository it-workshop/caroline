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

or

    cmake -G Ninja ..

if you want to build project with ninja build system.

Build project.

    make

or

    ninja

You can run unit tests.

    ./unit_tests

## Workflow

Each tracker issue is assigned to collaborator. Work on feature must be
subdivided to the work packages (wp). Each wp has own branch that must be merged
into upstream branch after review i.e. you must create a pull request for each
work package. branch can't be merged into the upstream if it has compilation
errors or failed unit tests on any platform.

### Names of branches

Each workpackage must be a branch with name ``wp/${issue-number}/${wp-number}``.
i.e. if issue 1357 has 3 workpackages, names of branches will be: ``wp/1357/1``,
``wp/1357/2``, ``wp/1357/3``.

### Commit message

Commit messages must begin with ``#${issue-number}: ``. Commit message must be
either oneline (up to 80 characters) or multiline with empty string after first
line.

## Code Conventions

Code conventions is based on the Google C++ Style guide:
http://google-styleguide.googlecode.com/svn/trunk/cppguide.xml

Additions:

 * We use C++11.

## Tests

Use GTest testing framework. Each unittest must be build into the unit_tests
target.

Currently project must be tested on the Linux, Windows, Mac OS X.
Supported toolkits:

 * GCC 4.8;
 * CLang 3.3.

## Scripting

If you need to write a script you must to use bash, perl, python.
Development tools must be saved in the tools directory. Build ``tools`` in the
``build`` directory.
