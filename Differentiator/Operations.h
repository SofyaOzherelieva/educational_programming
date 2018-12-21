//#define OPERATION(NUM, NAME, ARGC, CODE, DIFF_CODE) // можно добавить type


OPERATION(ADD_OP, "+", 2,
          { result = arg1 + arg2; },
          { return diffL + diffR; })

OPERATION(SUBTRACT_OP, "-", 2,
          { result = arg1 - arg2; },
          { return diffL - diffR; })

OPERATION(MULTIPLY_OP, "*", 2,
          { result = arg1 * arg2; },
          { return diffL * copyR + copyL * diffR; })

OPERATION(DIVIDE_OP, "/", 2,
          { result = arg1 / arg2; },
          { return (diffL * copyR - diffR * copyL) / (copyR * copyR); })

OPERATION(SIN_OP, "sin", 1,
          { result = std::sin(arg1); },
          { return diffL * cos(copyL); })

OPERATION(COS_OP, "cos", 1,
          { result = std::cos(arg1); },
          { return std::make_unique<ExprNode>(-1.) * diffL * sin(copyL); })

OPERATION(LOG_OP, "log", 1,
          { result = std::log(arg1); },
          { return diffL / copyL; })

OPERATION(CONST_OP, "", 0,
          {},
          { return std::make_unique<ExprNode>(0.); })

OPERATION(VAR_OP, "", 0,
          {},
          { return std::make_unique<ExprNode>(1.); })

OPERATION(EMPTY_OP, "", 0,
          {},
          {})