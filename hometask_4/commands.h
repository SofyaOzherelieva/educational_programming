
//#define COMMAND(NUM, NAME, ARGC, CODE)


COMMAND(END, "END", 0, {})

COMMAND(PUSH, "PUSH", 1, { stack.Push(command.argv_[0]); })

COMMAND(POP, "POP", 0, { stack.Pop(); })

COMMAND(ADD, "ADD", 0, { stack.Push(stack.PeekRemove() + stack.PeekRemove()); })
COMMAND(MUL, "MUL", 0, { stack.Push(stack.PeekRemove() * stack.PeekRemove()); })
COMMAND(SQRT, "SQRT", 0, { stack.Push(sqrt(stack.PeekRemove())); })

COMMAND(SUB, "SUB", 0, {
  double temp1 = stack.PeekRemove();
  double temp2 = stack.PeekRemove();
  stack.Push(temp2 - temp1);
})

COMMAND(DIV, "DIV", 0, {
  double temp1 = stack.PeekRemove();
  double temp2 = stack.PeekRemove();
  stack.Push(temp1 / temp2);
})

COMMAND(OUT, "OUT", 0, { std::cout << stack.PeekRemove() << "\n"; })

COMMAND(IN, "IN", 0, {
  double temp = NAN;
  std::cin >> temp;
  stack.Push(temp);
})



COMMAND(JUMP, "<-JUMP", 1, {
  return_pos.Push(position);
  position = command.argv_[0];
})

COMMAND(JUMPEQ, "<-JUMPEQ", 1, {
  double temp1 = stack.PeekRemove();
  double temp2 = stack.PeekRemove();
  if (fabs(temp1 - temp2) < EPS) {
    return_pos.Push(position);
    position = command.argv_[0];
  };
  stack.Push(temp2);
})

COMMAND(JUMPNEQ, "<-JUMPNEQ", 1, {
  double temp1 = stack.PeekRemove();
  double temp2 = stack.PeekRemove();
  if (fabs(temp1 - temp2) > EPS) {
    return_pos.Push(position);
    position = command.argv_[0];
  };
  stack.Push(temp2);
})

COMMAND(JUMPGR, "<-JUMPGR", 1, {
  double temp1 = stack.PeekRemove();
  double temp2 = stack.PeekRemove();
  if (temp2 >= temp1) {
    return_pos.Push(position);
    position = command.argv_[0];
  };
  stack.Push(temp2);
})

COMMAND(JUMPEMPT, "<-JUMPEMPT", 1, {
  if (stack.IsEmpty()) {
    return_pos.Push(position);
    position = command.argv_[0];
  };
})

COMMAND(TO, "->TO", 1, {})

COMMAND(RET, "RET", 0, {position = return_pos.PeekRemove();})

COMMAND(FSTR, "FSTR", 1, { registers[static_cast<int>(command.argv_[0])] = stack.PeekRemove(); })

COMMAND(FRTS, "FRTS", 1, { stack.Push(registers[command.argv_[0]]); })

COMMAND(DBL, "DBL", 0, {
  auto temp = stack.PeekRemove();
  stack.Push(temp);
  stack.Push(temp);
})

