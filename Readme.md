# Lab Assignment 3 – Parser with Flex and Bison

**Course:** COD7001 – Cornerstone Project (Systems Programming)  
**Lab Assignment:** 3  
**Topic:** Designing a Programming Language Parser using Flex and Bison  

---

## Problem Statement

The objective of this lab assignment is to design and implement a **parser for a simple imperative programming language** using **Flex (Lexical Analyzer)** and **Bison (Parser Generator)**.

The language supports:
- Variable declarations
- Assignments
- Arithmetic and relational expressions
- Conditional statements (`if / else`)
- Loop constructs (`while`)
- Block-based scoping using `{}`

The parser must tokenize input source code, validate it against a formally defined grammar, and construct an **Abstract Syntax Tree (AST)** for syntactically valid programs.

---

## Functional Requirements

1. **Lexical Analysis**
   - Tokenize identifiers, keywords, literals, operators, and delimiters
   - Ignore whitespace and comments

2. **Syntax Analysis**
   - Grammar implementation using Bison
   - Correct operator precedence and associativity
   - Support for:
     - Variable declarations
     - Assignments
     - Expressions
     - `if-else` statements
     - `while` loops
     - Blocks

3. **AST Construction**
   - Generate AST nodes during parsing
   - Represent program structure hierarchically

4. **Expression Evaluatiom**

---

## Non-Functional Requirements

- Clear grammar structure
- Meaningful syntax error messages
- No crashes on invalid input
- Modular and readable code

---

## Project Structure
- The Structure Used is Different than the defined structure in assignment pdf
```
lab-parser/
├── include/ # Header files
├── src/ # Core source files
├── lexer/ # Flex specification
├── parser/ # Bison grammar
├── tests/
│ ├── valid/ # Valid test programs
│ └── invalid/ # Invalid test programs
├── scripts/
│ └── run.sh # Test runner script
├── Makefile
└── README.md
```
---
## Cloning the project
To clone this project, use the following git command:
```bash
```bash
git clone https://github.com/Monit-Vaghela/COD7001-Assignment-3.git
```
---
## Using Makefile
The Makefile provides a simple interface for building and cleaning the project. The following are common make commands:
1. **Build the project**
Run the following command to compile the project files:
```bash
make
```
2. **Clean the project**
Run the following command to remove any generated files and clean up the project directory:
```bash
make clean
```
---
## Running the Script
To run the script, follow these steps:
1. Navigate to the directory containing the script:
```bash
cd scripts
```
2. Run the script with the following command:
```bash
./run.sh
```
- This creates 2 files 
1. **valid.txt** - cointains the solution for valid test cases
2. **invalid.txt** - cointains the solution for invalid test cases

