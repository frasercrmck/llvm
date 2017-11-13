// Generic Nemesys Format
class NemesysInst<dag outs, dag ins,
                  string asmstr, list<dag> pattern>
    : Instruction {
  field bits<32> Inst;

  let Namespace = "Nemesys";

  let Size = 4;

  bits<5> Opcode = 0;
  let Inst{31 - 27} = Opcode;

  let OutOperandList = outs;
  let InOperandList = ins;

  let AsmString = asmstr;
  let Pattern = pattern;
}
