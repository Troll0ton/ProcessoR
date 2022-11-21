
PRINT_ARG(2 * ARG_OFFSET, MASK_RAM | MASK_REG | MASK_IMM, "[r%cx + %d]\n",
                                                          (int) *(elem_t*)(Dasm->code + curr_pos + CMD_OFFSET) + 'a',
                                                          (int) *(elem_t*)(Dasm->code + curr_pos + CMD_OFFSET + ARG_OFFSET))

PRINT_ARG(1 * ARG_OFFSET, MASK_RAM | MASK_IMM,            "[%d]\n",
                                                          (int) *(elem_t*)(Dasm->code + curr_pos + CMD_OFFSET))

PRINT_ARG(1 * ARG_OFFSET, MASK_RAM | MASK_REG,            "[r%cx]\n",
                                                          (int) *(elem_t*)(Dasm->code + curr_pos + CMD_OFFSET) + 'a')

PRINT_ARG(1 * ARG_OFFSET, MASK_REG,                       "r%cx\n",
                                                          (int) *(elem_t*)(Dasm->code + curr_pos + CMD_OFFSET) + 'a')

PRINT_ARG(1 * ARG_OFFSET, MASK_IMM,                       "%d\n",
                                                          (int) *(elem_t*)(Dasm->code + curr_pos + CMD_OFFSET))
