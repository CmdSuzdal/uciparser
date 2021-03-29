# UciParser library

![CMake](https://github.com/CmdSuzdal/uciparser/actions/workflows/cmake.yml/badge.svg)


UCI Parser library: a C++ library to parse message in the [Universal Chess Interface protocol]. Mainly written to support the development of chess related software (e.g. Engines). It is part of the [Commander Suzdal] project

Developed using a TDD approach.

## How To build

Command Line (basic) instructions:

First of all, clone and build the project:

```bash
git clone --recurse-submodules https://github.com/CmdSuzdal/uciparser.git
mkdir build
cd build
cmake [cmake_options] ../cmdsuzdal
cmake --build .
```

Possible `cmake_options`:

 - `-G <Generator>`: the generator to use to build the project. Suggested generator: [Ninja].
 - `-DCMAKE_INSTALL_PREFIX=<installation_directory>`: specify the installation directory for the `install` target
- `-DENABLE_COVERAGE_TEST=ON`: to enable test coverage computation

Example:
```bash
cmake -G Ninja -DCMAKE_INSTALL_PREFIX=../_INSTALL_ -DENABLE_COVERAGE_TEST=ON ../uciparser
```

Execute tests:
```bash
ctest -V .
```

Compute test coverage:
```bash
cmake --build . --target libuciparser_coverage
```

See the test coverage results opening the `libuciparser_coverage/index.html` file.

Install the library to use it from another project:
```bash
cmake --build . --target install
```

[Universal Chess Interface protocol]: https://github.com/CmdSuzdal/doc/blob/master/reference/uci.md
[Commander Suzdal]: https://www.fadedpage.com/showbook.php?pid=20170826
[Ninja]: https://ninja-build.org/
