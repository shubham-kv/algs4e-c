
Data Structures and Algorithms written in C from [Algorithms (4th
Edition)](https://algs4.cs.princeton.edu/home/) textbook by *Sedgewick* &
*Wayne*.

## Index

Check [index.md](./index.md) for the chapter wise index of data structures and
algorithms.

## Getting started

### Requirements

Please ensure you have `cmake` (minimum version `4.1`), `make` (GNU Make
`3.81`), `criterion` (for testing) and `pkgconf` installed on your system.

### Installation

Clone this repository and `cd` into the created directory.

```bash
git clone https://github.com/shubham-kv/algs4-c/
cd algs4-c
```

### Running

The data structures & algorithms live in the `lib` directory and the test
executable programs that use them (called as clients) live in the `clients`
directory.

Build the project with `cmake`:

```bash
cmake -S . -B build
OR
cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON   # To generate compile_commands.json
```

or you can make a `build` directory, `cd` into `build` directory and run
`cmake` in that directory with no arguments.

In this case `cmake` makes `Makefile`s in the `build` directory for us to use.
Build a specific client for example the `graph_paths_client.c` in
`clients/graph/` by using the make command like the following:

```bash
make -C build graph_paths_client
```

or build the entire project:

```bash
make -C build
```

Find and run the executable from the `build/bin/` directory.

An example passing in the arguments:

```bash
./build/bin/graph_paths_client dfs tinyG.txt 0
```

### Testing

Tests live inside the `tests` directory and all the test files are prefixed
with a `test_`, for example the test file for `graph_paths_bfp.c` under
`lib/graph/graph_paths_bfp/` is `test_graph_paths_bfp.c` and to build it run:

```bash
make -C build test_graph_paths_bfp
```

or build all the test executables:

```bash
make -C build/tests
```

Simply run the output binary from the `build/bin/` directory to run your test
(similar to how you would run any other executable file).

Use `ctest` for a quick run of all the tests after building all the test
executables:

```bash
ctest --test-dir build
```

## Feedback / Contributions

Contributions are Welcome!  
Feel free to share your feedback or raise an issue/PR to make any contributions.

Cheers!
