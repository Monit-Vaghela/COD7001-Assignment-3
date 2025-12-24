#!/bin/bash

PARSER=./myparser

# Output files for valid and invalid test cases
VALID_OUTPUT="./scripts/valid.txt"
INVALID_OUTPUT="./scripts/invalid.txt"

# Create/empty the output files before starting
> "$VALID_OUTPUT"
> "$INVALID_OUTPUT"

echo "=============================="
echo "Running VALID test cases"
echo "=============================="

for file in tests/valid/*.var; do
    echo ">>> $file" >> "$VALID_OUTPUT"
    $PARSER "$file" >> "$VALID_OUTPUT" 2>&1  # Redirect both stdout and stderr
    echo -e "\n" >> "$VALID_OUTPUT"
done

echo "=============================="
echo "Running INVALID test cases"
echo "=============================="

for file in tests/invalid/*.var; do
    echo ">>> $file" >> "$INVALID_OUTPUT"
    $PARSER "$file" >> "$INVALID_OUTPUT" 2>&1  # Redirect both stdout and stderr
    echo -e "\n" >> "$INVALID_OUTPUT"
done

echo "=============================="
echo "Test cases completed"
echo "=============================="
