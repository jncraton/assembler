#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Define operations
typedef enum {Halt, LoadImmediate, Add, AddImmediate, And, Or, Xor, Jump, BranchOnEqual, Move, Invalid}  __attribute__ ((__packed__)) Operation;

// Define registers
typedef enum {R0, R1, R2, R3, R4, IP, FLAGS}  __attribute__ ((__packed__)) Register;

// Define instruction format
typedef struct {
  Operation op;
  Register src;
  Register dst;
  signed char immediate;
} Instruction;

void print_registers(int * registers) {
  // Helper function to print register contents for debugging
  printf("IP: %d ", registers[IP]);
  for (int i = 0; i<5; i++) {
    printf("R%d: %d ", i, registers[i]);
  }
  printf("\n");
}

char* instruction_names[10] = {"Halt", "LoadImmediate", "Add", "AddImmediate", "And", "Or", "Xor", "Jump", "BranchOnEqual", "Move"};


void print_instruction(Instruction inst) {
  // Helper function to print instruction information for debugging
  printf("%s src=%d dst=%d imm=%d\n", instruction_names[inst.op], inst.src, inst.dst, inst.immediate);
}

void execute(Instruction * instructions, int * registers) {
  Instruction inst = instructions[registers[IP]];

  while (inst.op != Halt) {
    if (inst.op == LoadImmediate) {
      registers[inst.dst] = inst.immediate;
    } else if (inst.op == Jump) {
      registers[IP] = inst.immediate-1;
    } else if (inst.op == BranchOnEqual) {
      if (registers[inst.src] == registers[inst.dst]) {
        registers[IP] = inst.immediate-1;
      }
    } else if (inst.op == Add) {
      registers[inst.dst] = registers[inst.src] + registers[inst.dst];
    } else if (inst.op == Move) {
      registers[inst.dst] = registers[inst.src];
    } else if (inst.op == And) {
      registers[inst.dst] = registers[inst.src] & registers[inst.dst];
    } else if (inst.op == Or) {
      registers[inst.dst] = registers[inst.src] | registers[inst.dst];
    } else if (inst.op == Xor) {
      registers[inst.dst] = registers[inst.src] ^ registers[inst.dst];
    } else if (inst.op == AddImmediate) {
      registers[inst.dst] = registers[inst.src] + inst.immediate;
    }

    registers[IP]++;
    inst = instructions[registers[IP]];
  }
}

Instruction * assembler(char* code) {
  /**
   * Returns a list of machine Instructions from an assembly language string
   */

  static Instruction instructions[256];

  char tokens[4][16];

  char *line = strtok(code, "\n");

  // Iterate over lines in the file
  while (line) {
    // Read opcodes and operands
    memset(tokens, 0, sizeof(tokens));
    sscanf(line, "%15s %15s %15s %15s", tokens[0], tokens[1], tokens[2], tokens[3]);
    line = strtok(NULL, "\n");

    // Handle comments and blank lines

    // If we reach this point, We are expecting a valid instruction

    // Process opcode

    // Process operands
  }

  return instructions;
}

/* 
Test code 

It is not recommended to modify any code below this point
*/

#define test_int_equal(exp, result) \
printf("Test %s == %d ", #exp, result); \
if ((exp) != result) { \
  printf("FAILED as %d.\n", exp); \
} else { \
  printf("succeeded.\n"); \
}

#define test_float_equal(exp, result) \
printf("Test %s == %f ", #exp, result); \
if ((exp) != result) { \
  printf("FAILED as %f.\n", exp); \
} else { \
  printf("succeeded.\n"); \
}

int main() {
  printf("Halt Tests\n");
  Instruction halt_instructions[4]={
    {Halt, 0, 0, 0}
  };
  int registers[6] = {0,0,0,0,0,0};
  execute(halt_instructions, registers);
  test_int_equal(registers[IP], 0);
  test_int_equal(registers[R0], 0);
  test_int_equal(registers[R1], 0);
  test_int_equal(registers[R2], 0);

  printf("Load Immediate Tests\n");
  Instruction load_imm_instructions[4]={
    {LoadImmediate, 0, R0, 1},
    {LoadImmediate, 0, R1, 80},
    {LoadImmediate, 0, R2, 13},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute(load_imm_instructions, registers);
  test_int_equal(registers[IP], 3);
  test_int_equal(registers[R0], 1);
  test_int_equal(registers[R1], 80);
  test_int_equal(registers[R2], 13);

  printf("Add Tests\n");
  Instruction add_instructions[4]={
    {LoadImmediate, 0, R0, 42},
    {LoadImmediate, 0, R1, 100},
    {Add, R0, R1, 0},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute(add_instructions, registers);
  test_int_equal(registers[IP], 3);
  test_int_equal(registers[R0], 42);
  test_int_equal(registers[R1], 142);
  test_int_equal(registers[R2], 0);

  printf("Add Immediate Tests\n");
  Instruction addi_instructions[6]={
    {AddImmediate, R0, R0, 1},
    {AddImmediate, R0, R0, 2},
    {AddImmediate, R1, R1, 21},
    {AddImmediate, R1, R1, 30},
    {AddImmediate, R1, R2, 1},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute(addi_instructions, registers);
  test_int_equal(registers[IP], 5);
  test_int_equal(registers[R0], 3);
  test_int_equal(registers[R1], 51);
  test_int_equal(registers[R2], 52);

  printf("Bitwise Tests\n");
  Instruction bitwise_instructions[9]={
    {LoadImmediate, 0, R0, 1},
    {LoadImmediate, 0, R1, 1},
    {And, R0, R1, 0},
    {LoadImmediate, 0, R2, 0},
    {Or, R0, R2, 0},
    {LoadImmediate, 0, R3, 0},
    {Xor, R0, R3, 0},
    {Xor, R0, R3, 0},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute(bitwise_instructions, registers);
  test_int_equal(registers[IP], 8);
  test_int_equal(registers[R0], 1);
  test_int_equal(registers[R1], 1);
  test_int_equal(registers[R2], 1);
  test_int_equal(registers[R3], 0);


  printf("Jump Tests\n");
  Instruction jump_instructions[4]={
    {LoadImmediate, 0, R0, 1},
    {Jump, 0, 0, 3},
    {LoadImmediate, 0, R1, 2},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute(jump_instructions, registers);
  test_int_equal(registers[IP], 3);
  test_int_equal(registers[R0], 1);
  test_int_equal(registers[R1], 0);
  test_int_equal(registers[R2], 0);

  printf("BranchOnEqual equal test\n");
  Instruction beq_instructions[5]={
    {LoadImmediate, 0, R0, 1},
    {LoadImmediate, 0, R1, 1},
    {BranchOnEqual, R0, R1, 4},
    {Halt, 0, 0, 0},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute(beq_instructions, registers);
  test_int_equal(registers[IP], 4);
  test_int_equal(registers[R0], 1);
  test_int_equal(registers[R1], 1);
  test_int_equal(registers[R2], 0);

  printf("BranchOnEqual not equal Test\n");
  Instruction beq2_instructions[5]={
    {LoadImmediate, 0, R0, 1},
    {LoadImmediate, 0, R1, 2},
    {BranchOnEqual, R0, R1, 4},
    {Halt, 0, 0, 0},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute(beq2_instructions, registers);
  test_int_equal(registers[IP], 3);
  test_int_equal(registers[R0], 1);
  test_int_equal(registers[R1], 2);
  test_int_equal(registers[R2], 0);

  printf("Move Tests\n");
  Instruction move_instructions[5]={
    {LoadImmediate, 0, R0, 42},
    {LoadImmediate, 0, R1, 100},
    {Move, R0, R2, 0},
    {Move, R1, R0, 0},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute(move_instructions, registers);
  test_int_equal(registers[IP], 4);
  test_int_equal(registers[R0], 100);
  test_int_equal(registers[R1], 100);
  test_int_equal(registers[R2], 42);

  printf("Extended Tests\n");
  Instruction ext_instructions[11]={
    {LoadImmediate, 0, R0, 1},
    {LoadImmediate, 0, R1, 12},
    {LoadImmediate, 0, R2, 100},
    {Add, R0, R1, 0},
    {Add, R2, R1, 0},
    {Add, R2, R1, 0},
    {LoadImmediate, 0, R0, -12},
    {Add, R0, R1, 0},
    {LoadImmediate, 0, R0, 1},
    {LoadImmediate, 0, R2, 3},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute(ext_instructions, registers);
  test_int_equal(registers[IP], 10);
  test_int_equal(registers[R0], 1);
  test_int_equal(registers[R1], 201);
  test_int_equal(registers[R2], 3);

  printf("Loop Test\n");
  Instruction loop_instructions[11]={
    {LoadImmediate, 0, R0, 0}, // Stays zero
    {LoadImmediate, 0, R1, -1}, // Stays -1
    {LoadImmediate, 0, R2, 11}, // Loop counter
    {LoadImmediate, 0, R3, 3}, // Stays 3
    {LoadImmediate, 0, R4, 0}, // Counting by 3s
    // Decrement loop counter
    {Add, R1, R2, 0}, // Instruction 5
    {BranchOnEqual, R2, R0, 9}, // Exit loop
    // Increment output
    {Add, R3, R4, 0},
    {Jump, 0, 0, 5},
    {Halt, 0, 0, 0}
  };
  memset(registers, 0, sizeof(registers));
  execute(loop_instructions, registers);
  test_int_equal(registers[IP], 9);
  test_int_equal(registers[R0], 0);
  test_int_equal(registers[R1], -1);
  test_int_equal(registers[R2], 0);
  test_int_equal(registers[R3], 3);
  test_int_equal(registers[R4], 30);

  printf("Assembler Test\n");
  char program[10000];

  FILE * f = fopen("test-program.asm", "r");
  fread(program, 1, 10000, f);
  
  Instruction * asm_instructions = assembler(program);
  memset(registers, 0, sizeof(registers));
  execute(asm_instructions, registers);
  test_int_equal(registers[IP], 9);
  test_int_equal(registers[R0], 0);
  test_int_equal(registers[R1], -1);
  test_int_equal(registers[R2], 0);
  test_int_equal(registers[R3], 3);
  test_int_equal(registers[R4], 30);

  printf("Fibonacci Test\n");

  f = fopen("fibonacci.asm", "r");
  fread(program, 1, 10000, f);
  
  asm_instructions = assembler(program);
  memset(registers, 0, sizeof(registers));

  memset(registers, 0, sizeof(registers));
  registers[R0] = 0;
  execute(asm_instructions, registers);
  test_int_equal(registers[R0], 0);

  memset(registers, 0, sizeof(registers));
  registers[R0] = 1;
  execute(asm_instructions, registers);
  test_int_equal(registers[R0], 1);

  memset(registers, 0, sizeof(registers));
  registers[R0] = 2;
  execute(asm_instructions, registers);
  test_int_equal(registers[R0], 1);

  memset(registers, 0, sizeof(registers));
  registers[R0] = 3;
  execute(asm_instructions, registers);
  test_int_equal(registers[R0], 2);

  memset(registers, 0, sizeof(registers));
  registers[R0] = 20;
  execute(asm_instructions, registers);
  test_int_equal(registers[R0], 6765);
}
