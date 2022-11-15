
PRINT_ARG(2, MASK_RAM | MASK_REG | MASK_IMM, "[r%cx + %d]\n", (int) Dasm->code[curr_pos + 1] + 'a', (int) Dasm->code[curr_pos + 2])

PRINT_ARG(1, MASK_RAM | MASK_IMM,            "[%d]\n",        (int) Dasm->code[curr_pos + 1])

PRINT_ARG(1, MASK_RAM | MASK_REG,            "[r%cx]\n",      (int) Dasm->code[curr_pos + 1] + 'a')

PRINT_ARG(1, MASK_REG,                       "r%cx\n",        (int) Dasm->code[curr_pos + 1] + 'a')

PRINT_ARG(1, MASK_IMM,                       "%d\n",          (int) Dasm->code[curr_pos + 1])
