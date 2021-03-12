Assembler
=========

This project requires the implementation of a very simple processor to execute instructions. The instruction format used is intended to be very simple (while not being space efficient).

The project extends a previous project to add an assembler and a program written in assembly to calculate Fibonacci numbers.

Instruction Format
------------------

Instructions are all 4 bytes. Each byte represents the following:

- Byte 0: Operation to execute
- Byte 1: Source register
- Byte 2: Destination register
- Byte 3: Immediate Value (signed)

Specifically, instructions are defined as:

```c
typedef struct {
  Operation op;
  Register src;
  Register dst;
  signed char immediate;
} Instruction;
```

Operations
----------

This simple processor supports the following operations:

1. Halt - Stop the processor
2. LoadImmedate - Store the value in `immediate` to register `dst`.
3. Add - Add the value from register `src` to the value in register `dst` and store the result in `dst`.
4. AddImmediate - Add the value in `immediate` to register `src` and store the result in register `dst`.
5. Move - Copy the contents of register `src` to register `dst`.
6. And - Bitwise `and` the value from register `src` to the value in register `dst` and store the result in `dst`.
7. Or - Bitwise `or` the value from register `src` to the value in register `dst` and store the result in `dst`.
8. Xor - Bitwise `xor` the value from register `src` to the value in register `dst` and store the result in `dst`.
9. Jump - Jump to the absolute position stored in the `immediate` value.
10. BranchOnEqual - Jump to the absolute position stored in the `immediate` value if and only if the values in the `src` and `dst` registers are equal.

It has no access to external memory or I/O.

Running
-------

Once the `execute` function has been implemented, the processor can be compiled and tested by invoking:

```
make
```
