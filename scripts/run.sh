#!/bin/bash

PARSER=./parser

echo "=============================="
echo "Running VALID test cases"
echo "=============================="

for file in tests/valid/*.var; do
    echo ">>> $file"
    $PARSER $file
    echo
done

echo "=============================="
echo "Running INVALID test cases"
echo "=============================="

for file in tests/invalid/*.var; do
    echo ">>> $file"
    $PARSER $file
    echo
done
