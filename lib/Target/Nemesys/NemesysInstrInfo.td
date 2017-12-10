include "NemesysInstrFormats.td"

class CC<int v, string s> : Enum {
  let Val = v;
  let String = s;
  let PrintMethod = "getCCString";
  let MatchMethod = "matchCCString";
}

multiclass ImmOp<int b> {
  def AsmOperand : AsmOperandClass {
    let Name = NAME;
    let RenderMethod = "addImmOperands";
    let DiagnosticString = "operand must be "#b#"-bit signed immediate";
  }

  def NAME : Operand<i32>, ImmLeaf<i32, "return isInt<"#b#">(Imm);"> {
    let ParserMatchClass =
        !cast<AsmOperandClass>(!strconcat(NAME, "AsmOperand"));
  }
}

defm ImmSExt6  : ImmOp<6>;
defm ImmSExt16 : ImmOp<16>;

multiclass ALUBinOp<bits<5> opc, string mnemonic, SDNode node> {
  def r : ALUInst<opc,
                  (outs RC:$rZ), (ins RC:$rA, RC:$rB),
                  !strconcat(mnemonic, " $rA, $rB, $rZ"),
                  [(set RC:$rZ, (node RC:$rA, RC:$rB))]> {
    bits<5> rB;
    let Inst{26}    = 0;
    let Inst{4 - 0} = rB;
  }

  def i : ALUInst<opc,
                  (outs RC:$rZ), (ins RC:$rA, ImmSExt6:$rB),
                  !strconcat(mnemonic, " $rA, $rB, $rZ"),
                  [(set RC:$rZ, (node RC:$rA, ImmSExt6:$rB))]> {
    bits<6> rB;
    let Inst{26}    = 1;
    let Inst{25}    = rB{5};
    let Inst{4 - 0} = rB{4 - 0};
  }
}

defm ADD : ALUBinOp<0x0, "add", add>;
defm SUB : ALUBinOp<0x1, "sub", sub>;
defm MPY : ALUBinOp<0x2, "mpy", mul>;

defm AND : ALUBinOp<0x5, "and", and>;
defm OR  : ALUBinOp<0x6, "or",  or>;
defm XOR : ALUBinOp<0x7, "xor", xor>;

defm SHL : ALUBinOp<0xA, "shl", shl>;
defm SRA : ALUBinOp<0xB, "sra", sra>;
defm SRL : ALUBinOp<0xC, "srl", srl>;

def CondCodeAsmOperand : AsmOperandClass {
  let Name = "CondCode";
  let RenderMethod = "addCondCodeOperands";
  let DiagnosticString = "operand must be a condition code";
}

def CondCode : Operand<i32>, ImmLeaf<i32, "return isInt<"#b#">(Imm);"> {
  let PrintMethod = "printCondCode";
  let ParserMatchClass = CondCodeAsmOperand;
}

def CMPr : CMPInst<0xD, (outs PC:$pZ),
                   (ins RC:$rA, RC:$rB, CondCode:$cc),
                   "cmp.$cc $rA, $rB, $pZ", []> {
  bits<5> rB;
  let Inst{4 - 0} = rB;
}

def CMPi : CMPInst<0xD, (outs PC:$pZ),
                   (ins RC:$rA, ImmSExt6:$rB, CondCode:$cc),
                   "cmp.$cc $rA, $rB, $pZ", []> {
  bits<6> rB;
  let Inst{26} = 1;
  let Inst{25} = rB{5};
  let Inst{4 - 0} = rB{4 - 0};
}

multiclass CMPPat<SDPatternOperator op, CC cc> {
  def : Pat<(op RC:$rA, RC:$rB), (CMPr $rA, $rB, cc.Val)>;
  def : Pat<(op RC:$rA, ImmSExt6:$rB), (CMPi $rA, $rB, cc.Val)>;
}

def EQ  : CC<0b0000, "eq">;
def NE  : CC<0b0001, "ne">;
def LT  : CC<0b0010, "lt">;
def LE  : CC<0b0011, "le">;
def ULT : CC<0b0100, "ult">;
def ULE : CC<0b0101, "ule">;

def GE  : CC<0b1010, "ge">;
def GT  : CC<0b1011, "gt">;
def UGE : CC<0b1100, "uge">;
def UGT : CC<0b1101, "ugt">;

defm : CMPPat<seteq,  EQ>;
defm : CMPPat<setne,  NE>;
defm : CMPPat<setlt,  LT>;
defm : CMPPat<setle,  LE>;
defm : CMPPat<setult, ULT>;
defm : CMPPat<setule, ULE>;

defm : CMPPat<setge,  GE>;
defm : CMPPat<setgt,  GT>;
defm : CMPPat<setuge, UGE>;
defm : CMPPat<setugt, UGT>;

def MOV  : MOVInst<0xF, (outs RC:$rZ), (ins ImmSExt16:$rA),
                   "mov $rA, $rZ",
                   [(set RC:$rZ, ImmSExt16:$rA)]>;
def MOVH : MOVInst<0x10, (outs RC:$rZ), (ins ImmSExt16:$rA),
                   "movh $rA, $rZ", []>;

def PCRel16AsmOperand : AsmOperandClass {
  let Name = "PCRel16";
  let PredicateMethod = "isImm";
  let RenderMethod = "addImmOperands";
  let ParserMethod = "parsePCRel16";
}

def brtarget : Operand<OtherVT> {
  let EncoderMethod = "getPCRel16Value";
  let ParserMatchClass = PCRel16AsmOperand;
}

def NegatablePredAsmOperand : AsmOperandClass {
  let Name = "NegatablePred";
  let PredicateMethod = "isNegatablePred";
  let RenderMethod = "addNegatablePredOperands";
  let ParserMethod = "parseNegatablePred";
  let DiagnosticString = PC.DiagnosticString;
}

def NegatablePred : Operand<i1> {
  let MIOperandInfo = (ops PC:$pA, i32imm:$inverse);
  let PrintMethod = "printNegatablePred";
  let DecoderMethod = "decodeNegatablePred";
  let EncoderMethod = "getNegatablePredValue";
  let ParserMatchClass = NegatablePredAsmOperand;
}

def BR : NemesysInst<0x13, (outs), (ins NegatablePred:$pA, brtarget:$tgt),
                     "br $pA, $tgt", []> {
  bits<4> pA;
  bits<16> tgt;
  let isBranch = 1;
  let Inst{19 - 16} = pA;
  let Inst{15 - 0} = tgt;
}

def CALL : NemesysInst<0x15, (outs), (ins brtarget:$tgt),
                       "call $tgt", []> {
  bits<16> tgt;
  let isCall = 1;
  let Inst{15 - 0} = tgt;
}

def RET : NemesysInst<0x16, (outs), (ins), "ret", []> {
  let isReturn = 1;
  let isBarrier = 1;
  let isTerminator = 1;
}

def HALT : NemesysInst<0x14, (outs), (ins), "halt", []> {
  let hasSideEffects = 1;
}
