#ECE 5760 - Project 1
##Erik Sargent and Weston Jensen

To build the benchmarks, `cd` into the `benchmarks` directory, and run `make`. This will generate all of the benchmark files. 

To automatically run all benchmarks, run `make benchmark` inside the `benchmarks` directory.

Individual tests can be found inside the `makefile` in the root directory of the project. All code files are currently configured for benchmarking, but the search space can be expanded in each `MD5_*.h` file, and the target hash can be configured at the top of every `*.c`.
