
PARSE_ARG(2, MASK_RAM | MASK_REG | MASK_IMM, "%*s [r%cx + %lg]", &Arg->reg_sym, &Arg->value)

PARSE_ARG(1, MASK_RAM | MASK_IMM,            "%*s [%lg]",        &Arg->value)

PARSE_ARG(1, MASK_RAM | MASK_REG,            "%*s [r%cx]",       &Arg->reg_sym)

PARSE_ARG(1, MASK_REG,                       "%*s r%cx",         &Arg->reg_sym)

PARSE_ARG(1, MASK_IMM,                       "%*s %lg",          &Arg->value)


