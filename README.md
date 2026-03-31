# Student Split Benchmark v1.0

This project implements the final **v1.0** release for the student categorization task. It benchmarks three STL containers:

- `std::vector`
- `std::list`
- `std::deque`

It also supports the two required splitting strategies:

1. **Strategy 1**: create **two new containers** (`failed` and `passed`).
2. **Strategy 2**: create **one new container** and keep the remaining students in the base container after partitioning.

The program uses algorithms from `<algorithm>` including:

- `std::copy_if`
- `std::stable_partition`
- `std::move`

## Release history

### v0.1
Basic version with file reading, final grade calculation, and output formatting.

### v0.25
Added sorting/categorization support and performance testing idea.

### v1.0
Final optimized release:
- supports `vector`, `list`, and `deque`
- supports both required strategies
- uses `<algorithm>` functions instead of manual loops for splitting
- includes `Makefile` and `CMakeLists.txt`
- writes `passed` and `failed` result files
- prints execution timing for read, split, write, and total steps

## Results comment

Typical observation:
- **Vector** is usually the fastest overall because of cache-friendly memory layout.
- **Deque** is often close to vector, but usually a bit slower.
- **List** usually loses in total speed despite fast splicing, because linked-list traversal is less cache-efficient.
- **Strategy 2** is usually more memory-efficient because it avoids duplicating both groups into two new containers.
- `std::stable_partition` is a clean and competitive choice for the final implementation.

Your exact numbers depend on the size of `studentai.txt`, compiler optimization level, and machine performance.

## Build and usage guide

### Option 1: Make (Unix-like OS)

```bash
make
./student_split studentai.txt
```

### Option 2: CMake (any OS)

```bash
cmake -S . -B build
cmake --build build --config Release
./build/student_split studentai.txt
```

On Windows with Visual Studio, the executable may be inside `build/Release/`.

## Program flow

1. Start the program.
2. Choose the container:
   - `1` = vector
   - `2` = list
   - `3` = deque
   - `4` = all containers
3. Choose the final grade mode:
   - `1` = average
   - `2` = median
4. Choose the split strategy:
   - `1` = two new containers
   - `2` = one new container
5. The program reads students from `studentai.txt`.
6. The program splits them into `passed` and `failed` groups.
7. Result files are written to the `output/` directory.
8. The console prints timing statistics.

## Input file format

Example:

```text
Vardas Pavarde ND1 ND2 ND3 Egz.
Jonas Jonaitis 8 9 10 9
Ona Onaite 4 5 3 4
```

The last number is treated as the exam grade. All previous grades on the line are treated as homework grades.

## Output files

The program generates files such as:

- `output/failed_vector_strategy1_average.txt`
- `output/passed_vector_strategy1_average.txt`
- `output/failed_list_strategy2_median.txt`
- `output/passed_deque_strategy2_average.txt`

## Repository hygiene

Keep only source code and required project files in GitHub:

- `include/`
- `src/`
- `CMakeLists.txt`
- `Makefile`
- `README.md`
- `studentai.txt`

Do not commit IDE folders, build folders, or temporary output files.
