# Boolean Satisfiability Solver

Authors: Timur Ibrayev, Nikhil Sunil Chhabria

## Directory Overview

main.cpp                                      Main file
data_types.h                                  SAT namespace definition
parse_cnf.h                                   Parser for input .cnf file
sat_solver.h                                  DPLL algorithm with randomization and geometric restarts (RGR) heuristic
benchmarks/uniform_random_3-sat/              Uniform Random 3-SAT (each folder is named as sat/unsat_<number of variables>_<number of clauses>, and has 3 instances, named 01.cnf, 02.cnf, and 03.cnf)
benchmarks/dimacs_benchmark_instances/aim/    Artificially generated Random 3-SAT (each folder is named as sat/unsat_<number of variables>_<number of clauses>, and has 3 instances, named 01.cnf, 02.cnf, and 03.cnf)
benchmarks/dimacs_benchmark_instances/phole/  Pigeon hole problem (3 instances, named 01.cnf, 02.cnf, and 03.cnf, all unsatisfiable)
doc/                                          Documentation

## Compilation

> g++ main.cpp -o mySAT

## Usage

1. Without RGR heuristic: mySAT <file name>
                 Example: mySAT benchmarks/uniform_random_3-sat/sat_50_218/01.cnf

2. With RGR heuristic (default values): mySAT <file name> --enable_restarts
                               Example: mySAT benchmarks/uniform_random_3-sat/sat_50_218/01.cnf --enable_restarts

3. With RGR heuristic (user-defined values): mySAT <file name> --enable_restarts <k value>(integer) <a value>(float)
                                    Example: mySAT benchmarks/uniform_random_3-sat/sat_50_218/01.cnf --enable_restarts 90 1.4

================================================================================
