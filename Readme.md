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

---

## Non-Functional Requirements

- Clear grammar structure
- Meaningful syntax error messages
- No crashes on invalid input
- Modular and readable code

---

## Project Structure
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

## How to Build

```bash
make
./parser <input-file>
./scripts/run.sh
```