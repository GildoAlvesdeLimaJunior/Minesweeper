# Minesweeper SAT Formalization & Solver

[![Academic Project](https://img.shields.io/badge/Academic-UFCA-blue.svg)](https://www.ufca.edu.br/)
[![Language](https://img.shields.io/badge/Language-C%2B%2B17-blue.svg)](https://isocpp.org/)
[![Format](https://img.shields.io/badge/Format-DIMACS%20CNF-green.svg)](https://en.wikipedia.org/wiki/Conjunctive_normal_form)
[![SAT Solver](https://img.shields.io/badge/Solver-CaDiCaL%20%2F%20Kissat-red.svg)](https://github.com/arminbiere/cadical)

This repository contains the mathematical formalization, DIMACS generator, and experimental framework for reducing **Minesweeper board states** to the **Boolean Satisfiability Problem (SAT)** in **Conjunctive Normal Form (CNF)**. Developed for the *Lógica para Ciência da Computação* course at Universidade Federal do Cariri (UFCA).

---

## 📌 Executive Summary

Minesweeper is a classic NP-complete decision problem when generalized to an arbitrary $n \times m$ grid. The core objective of this project is to model local board constraints (numbered cells specifying the exact quantity of adjacent bombs) into Propositional Logic formulas and verify board consistency/deductions using modern industrial-grade SAT solvers such as **CaDiCaL** and **Kissat**.

---

## 🧮 Mathematical Formalization

### 1. Propositional Variables

Let $G = (R \times C)$ represent an $R \times C$ board grid. For each cell at row $i \in \{1, \dots, R\}$ and column $j \in \{1, \dots, C\}$, we define a boolean variable $x_{i,j}$:

$$x_{i,j} = \begin{cases} 1 & \text{if cell }(i,j) \text{ contains a mine} \\ 0 & \text{otherwise} \end{cases}$$

For linearized DIMACS mapping, two-dimensional coordinates $(i, j)$ are mapped to a single integer variable index $v \in \{1, \dots, R \cdot C\}$:

$$v = (i - 1) \cdot C + j$$

### 2. CNF Clause Families

For a revealed cell $(i, j)$ containing a number $N \in \{0, 1, \dots, 8\}$, let $V = \{y_1, y_2, \dots, y_m\}$ denote the set of its $m$ unopened adjacent neighbors ($m \le 8$).

To enforce that **exactly $N$** of these neighbors contain mines, we decompose the cardinality constraint into two sub-conditions:

$$\text{Exact}(N, V) \iff \text{AtLeast}(N, V) \land \text{AtMost}(N, V)$$

#### A. At Least $N$ Mines

To ensure at least $N$ mines exist among $m$ neighbors, no subset of size $(m - N + 1)$ can all be safe simultaneously:

$$\bigwedge_{S \subseteq V, \, \vert{}S\vert{} = m - N + 1} \left( \bigvee_{y \in S} y \right)$$

* **Clause Count:** $\binom{m}{m - N + 1}$ clauses.

#### B. At Most $N$ Mines

To ensure no more than $N$ mines exist, no subset of size $(N + 1)$ can all be mines simultaneously:

$$\bigwedge_{S \subseteq V, \, \vert{}S\vert{} = N + 1} \left( \bigvee_{y \in S} \neg y \right)$$

* **Clause Count:** $\binom{m}{N + 1}$ clauses.

---

## 📁 Repository Structure

```text
.
├── src/
│   ├── generator.cpp      # C++ source code to output DIMACS CNF files
│   └── decoder.cpp        # C++ decoder to reconstruct board from SAT assignments
├── Makefile               # Build script for generator and decoder
├── instances/             # Pre-generated .cnf benchmarks (easy to hard)
├── results/               # Solver logs and execution time benchmarks
├── docs/                  # Project report and mathematical formulations
└── README.md              # Project documentation

```

---

## 🚀 Getting Started

### Prerequisites

* **C++17 Compiler** (`g++` or `clang++`)


* **Make** build utility
* **CaDiCaL** or **Kissat** SAT Solver



To compile CaDiCaL on Linux/macOS or WSL:

```bash
git clone https://github.com/arminbiere/cadical.git
cd cadical
./configure && make

```

### Building the Project

Compile the C++ generator and decoder executables using `make`:

```bash
make

```

*(Or compile manually using `g++ -O3 -std=c++17 src/generator.cpp -o generator`)*

### Usage Workflow

#### 1. Generate DIMACS CNF Instance

Generate a CNF instance file from a board configuration:

```bash
./generator --rows 8 --cols 8 --mines 10 > instances/minesweeper_8x8.cnf

```

#### 2. Verify DIMACS Header Integrity

The C++ generator automatically checks that the declared number of variables and clauses in the header (`p cnf VARS CLAUSES`) matches the actual clause count:

```text
c Minesweeper SAT Instance Generated for UFCA Logic Course
p cnf 64 128
1 2 3 0
-1 -2 -3 0
...

```

#### 3. Run SAT Solver

Execute the solver on the generated instance and pipe the output:

```bash
time cadical instances/minesweeper_8x8.cnf > results/solution_8x8.log

```

#### 4. Decode SAT Solution

Reconstruct the board solution from the SAT solver's model output:

```bash
./decoder results/solution_8x8.log

```

---

## 📊 Experimental Results

Experiments were performed on various grid dimensions to analyze execution time scaling and formula growth.

| Instance ID | Grid Size | Vars | Clauses | Result | Execution Time (s) |
| --- | --- | --- | --- | --- | --- |
| `inst_01_easy` | $4 \times 4$ | 16 | 42 | **SAT** | 0.002s |
| `inst_02_med` | $8 \times 8$ | 64 | 210 | **SAT** | 0.005s |
| `inst_03_hard` | $16 \times 16$ | 256 | 1,120 | **SAT** | 0.018s |
| `inst_04_unsat` | $8 \times 8$ | 64 | 240 | **UNSAT** | 0.004s |
| `inst_05_large` | $30 \times 16$ | 480 | 2,890 | **SAT** | 0.045s |

---

## 🎓 Academic Details & Authors

* **Institution:** Universidade Federal do Cariri (UFCA)


* **Center:** Centro de Ciências e Tecnologia (CCT)


* **Course:** Ciência da Computação


* **Discipline:** Lógica para Ciência da Computação


* **Professor:** Dr. Luis Henrique Bustamante


* **Authors:**
* Gildo Alves de Lima Júnior


* Gilvan Alves Pastor Júnior





---

## 📚 References

1. **Biere, A., Heule, M., van Maaren, H., & Walsh, T.** (2021). *Handbook of Satisfiability* (2nd ed.). IOS Press.


2. **Avigad, J.** (2022). *Mathematical Logic and Computation*. Cambridge University Press.
