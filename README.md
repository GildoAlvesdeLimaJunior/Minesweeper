# Minesweeper SAT Formalization & CNF Encoder

[![Academic Project](https://img.shields.io/badge/Academic-UFCA-blue.svg)](https://www.ufca.edu.br/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![Format](https://img.shields.io/badge/Format-DIMACS%20CNF-green.svg)](https://en.wikipedia.org/wiki/Conjunctive_normal_form)
[![SAT Solver](https://img.shields.io/badge/Solver-CaDiCaL%20%2F%20Kissat-red.svg)](https://github.com/arminbiere/cadical)

This repository contains the mathematical formalization and a **C++ CNF encoder** that reduces **Minesweeper board states** to the **Boolean Satisfiability Problem (SAT)** in **Conjunctive Normal Form (CNF)**. Developed for the *Lógica para Ciência da Computação* course at Universidade Federal do Cariri (UFCA).

---

## Table of Contents

- [Executive Summary](#-executive-summary)
- [Mathematical Formalization](#-mathematical-formalization)
- [Repository Structure](#-repository-structure)
- [Getting Started](#-getting-started)
- [Experimental Results](#-experimental-results)
- [Academic Details](#-academic-details)
- [References](#-references)

---

## 📌 Executive Summary

Minesweeper is a classic NP-complete decision problem when generalized to an arbitrary $n \times m$ grid. The core objective of this project is to model local board constraints (numbered cells specifying the exact quantity of adjacent bombs) into Propositional Logic formulas and verify board consistency/deductions using modern industrial-grade SAT solvers such as **CaDiCaL** and **Kissat**.

The `cnf_generator` executable encodes a given board configuration into a DIMACS CNF instance. The SAT solver is run externally; once a model is produced, the original board can be reconstructed from the variable assignments (decoder planned).

---

## 🧮 Mathematical Formalization

### 1. Propositional Variables

Let $G = (R \times C)$ represent an $R \times C$ board grid. For each cell at row $i \in \{1, \dots, R\}$ and column $j \in \{1, \dots, C\}$, we define a boolean variable $x_{i,j}$:

$$x_{i,j} = \begin{cases} 1 & \text{if cell }(i,j) \text{ contains a mine} \\ 0 & \text{otherwise} \end{cases}$$

For linearized DIMACS mapping, two-dimensional coordinates $(i, j)$ are mapped to a single integer variable index $v \in \{1, \dots, R \cdot C\}$:

$$v = (i - 1) \cdot C + j$$

In the implementation (0-indexed `x`, `y`), this corresponds to `x * C + y + 1`.

### 2. CNF Clause Families

For a revealed cell $(i, j)$ containing a number $N \in \{0, 1, \dots, 8\}$, let $V = \{y_1, y_2, \dots, y_m\}$ denote the set of its $m$ unopened adjacent neighbors ($m \le 8$).

First, every revealed cell is forced to be mine-free with the unit clause:

$$(\neg x_{i,j})$$

Then, to enforce that **exactly $N$** of the unknown neighbors contain mines, we decompose the cardinality constraint into two sub-conditions:

$$\text{Exact}(N, V) \iff \text{AtLeast}(N, V) \land \text{AtMost}(N, V)$$

#### A. At Least $N$ Mines

To ensure at least $N$ mines exist among $m$ neighbors, no subset of size $(m - N + 1)$ can all be safe simultaneously:

$$\bigwedge_{S \subseteq V, \, \vert{}S\vert{} = m - N + 1} \left( \bigvee_{y \in S} y \right)$$

- **Clause Count:** $\binom{m}{m - N + 1}$ clauses.

#### B. At Most $N$ Mines

To ensure no more than $N$ mines exist, no subset of size $(N + 1)$ can all be mines simultaneously:

$$\bigwedge_{S \subseteq V, \, \vert{}S\vert{} = N + 1} \left( \bigvee_{y \in S} \neg y \right)$$

- **Clause Count:** $\binom{m}{N + 1}$ clauses.

### 3. Inconsistent Boards

If a revealed cell declares $N > m$ (more mines than available unknown neighbors), the constraint is impossible to satisfy. The encoder emits an **empty clause**, making the whole formula **UNSAT** — a correct indication that the board state is inconsistent.

---

## 📁 Repository Structure

```text
.
├── src/
│   ├── cnf_generator.cpp   # C++ CNF encoder (board state → DIMACS CNF)
│   └── test_sat.cnf        # Sample generated instance
└── README.md               # Project documentation
```

> **Planned (not yet implemented):**
> - `src/decoder.cpp` — reconstruct board from a SAT solver's model output
> - `Makefile` — build script for encoder and decoder
> - `instances/` — CNF benchmarks (easy to hard)
> - `results/` — solver logs and execution time benchmarks
> - `docs/` — project report and detailed mathematical formulations

---

## 🚀 Getting Started

### Prerequisites

- **C++17 Compiler** (`g++` or `clang++`)
- **CaDiCaL** or **Kissat** SAT Solver

To compile CaDiCaL on Linux/macOS or WSL:

```bash
git clone https://github.com/arminbiere/cadical.git
cd cadical
./configure && make
```

### Building the Project

Compile the CNF encoder:

```bash
g++ -O3 -std=c++17 src/cnf_generator.cpp -o src/cnf_generator
```

### Input Format

The encoder reads from standard input:

1. Two integers `n m` — board dimensions.
2. An `n × m` matrix of integers:
   - `-1` for **unknown** (unopened) cells;
   - `0`–`8` for **revealed** cells (the number of adjacent mines).

Example board (`3 × 3`, center cell reveals `1`):

```text
3 3
-1 -1 -1
-1 1 -1
-1 -1 -1
```

### Usage Workflow

#### 1. Generate DIMACS CNF Instance

```bash
./src/cnf_generator < board.txt > out.cnf
```

Output:

```text
p cnf 9 30
-5 0
9 8 7 6 4 3 2 1 0
-9 -8 0
...
```

The header (`p cnf VARS CLAUSES`) reports the exact number of variables ($R \cdot C$) and the total number of clauses generated.

#### 2. Run SAT Solver

Execute the solver on the generated instance and pipe the output:

```bash
time cadical out.cnf > solution.log
```

#### 3. Decode SAT Solution *(planned)*

Reconstruct the board solution from the SAT solver's model output with the upcoming `decoder`:

```bash
./decoder solution.log
```

---

## 📊 Experimental Results

Benchmark data is **pending**. Once `instances/` (generated CNF benchmarks) and `results/` (solver logs) are added, this section will be populated with execution time and formula growth measurements across board dimensions.

---

## 🎓 Academic Details

- **Institution:** Universidade Federal do Cariri (UFCA)
- **Center:** Centro de Ciências e Tecnologia (CCT)
- **Course:** Ciência da Computação
- **Discipline:** Lógica para Ciência da Computação
- **Professor:** Dr. Luis Henrique Bustamante
- **Authors:**
  - Gildo Alves de Lima Júnior
  - Gilvan Alves Pastor Júnior

---

## 📚 References

1. **Biere, A., Heule, M., van Maaren, H., & Walsh, T.** (2021). *Handbook of Satisfiability* (2nd ed.). IOS Press.
2. **Avigad, J.** (2022). *Mathematical Logic and Computation*. Cambridge University Press.
