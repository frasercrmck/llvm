// Generic Nemesys Format
class NemesysInst<bits<5> opc,
                  dag outs, dag ins,
                  string asmstr, list<dag> pattern>
    : Instruction {
  field bits<32> Inst;
  field bits<32> SoftFail;

  let Namespace = "Nemesys";

  let Size = 4;

  let Inst{31 - 27} = opc;

  let OutOperandList = outs;
  let InOperandList = ins;

  let AsmString = asmstr;
  let Pattern = pattern;
}

class ALUInst<bits<5> opc, dag outs, dag ins,
              string asmstr, list<dag> pattern>
    : NemesysInst<opc, outs, ins, asmstr, pattern> {
  bits<5> rZ;
  bits<5> rA;
  let Inst{26 - 21} = 0;
  let Inst{20 - 16} = rZ;
  let Inst{15 - 10} = 0;
  let Inst{9 - 5} = rA;
  let Inst{4 - 0} = 0;
}

class CMPInst<bits<5> opc, dag outs, dag ins,
              string asmstr, list<dag> pattern>
    : NemesysInst<opc, outs, ins, asmstr, pattern> {
  bits<3> pZ;
  bits<4> cc;
  bits<5> rA;
  let Inst{26 - 19} = 0;
  let Inst{18 - 16} = pZ;
  let Inst{15 - 14} = 0;
  let Inst{13 - 10} = cc;
  let Inst{9 - 5} = rA;
  let Inst{4 - 0} = 0;
}

class MOVInst<bits<5> opc, dag outs, dag ins,
              string asmstr, list<dag> pattern>
    : NemesysInst<opc, outs, ins, asmstr, pattern> {
  bits<5> rZ;
  bits<16> rA;
  let Inst{20 - 16} = rZ;
  let Inst{15 - 0} = rA;
}
