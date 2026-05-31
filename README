# LargeNumber

A C++ project for arbitrary-precision integer arithmetic and expression evaluation using a custom `LN` large-number type.

## Overview

This repository implements a big integer library and a command-line evaluator that reads one operator or operand per line from an input file and writes results to an output file.

The evaluator supports:
- Large integers of arbitrary length
- Binary operations: `+`, `-`, `*`, `/`, `%`
- Unary operations: `_` (negation), `~` (integer square root)
- Comparison operations: `==`, `!=`, `<`, `<=`, `>`, `>=`

## Files

- `main.cpp` - program entry point; reads input, evaluates operations using a stack, and writes output.
- `LN.h` / `LN.cpp` - implementation of the `LN` class for large integer arithmetic and comparisons.
- `utility.h` / `utility.cpp` - helper routines for arithmetic on string-based digit arrays and token classification.
- `return_codes.h` - common error codes used by the application.

## Usage

Run the program with an input file and an output file path:

```powershell
.\LargeNumber.exe input.txt output.txt
```

### Input format

The input file must contain a series of operations in postfix form. Each input line must be one of the following:
- A decimal integer literal (positive or negative)
- A unary operator: `_` for negation, `~` for integer square root
- A binary operator: `+`, `-`, `*`, `/`, `%`
- A comparator: `==`, `!=`, `<`, `<=`, `>`, `>=`

The program processes input as a stack-based evaluator. Numbers push onto the stack, and operators consume operands from the top of the stack.

### Output

The output file contains the remaining stack values printed from top to bottom, one per line.
- Negative results are prefixed with `-`
- `NaN` is printed for invalid operations such as division by zero or square root of a negative value

## Example

Input file:

```text
314159265358979323846264338327
271828182845904523536
+
13
/
```

Output file:

```text
24166097356215962053243758604
```

## Notes

- The program expects exactly two command-line arguments. If this is not provided, it exits with `ERROR_INVALID_PARAMETER`.
- Invalid input data or insufficient operands causes the program to return `ERROR_INVALID_DATA`.
- The implementation uses custom heap allocation for digit storage and avoids external big integer libraries.
