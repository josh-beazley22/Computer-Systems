typedef enum {
    NOOP,
    READ,
    WRITE,
    ADD,
    SUB,
    MUL,
    DIV,
    ADDF,
    SUBF,
    MULF,
    DIVF,
} Instruction;

#define NUM_INSTRUCTIONS 11

static const int instruction_cycles[NUM_INSTRUCTIONS] = {
    [NOOP]  = 0,
    [READ]  = 1,
    [WRITE] = 1,
    [ADD]   = 1,
    [SUB]   = 1,
    [MUL]   = 1,
    [DIV]   = 1,
    [ADDF]  = 4,
    [SUBF]  = 4,
    [MULF]  = 4,
    [DIVF]  = 8,
};

#define PIPELINE_SIZE   5

typedef struct {
    Instruction cpu_stage[PIPELINE_SIZE];
    int execute_step;
} CPU;

#define IF_STAGE    0
#define ID_STAGE    1
#define EX_STAGE    2
#define ME_STAGE    3
#define WB_STAGE    4

typedef struct {
    Instruction* instructions;
    int size;
    int current;
} Program;

