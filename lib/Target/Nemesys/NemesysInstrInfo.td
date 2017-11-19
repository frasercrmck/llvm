include "NemesysInstrFormats.td"

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

multiclass CMPPat<SDPatternOperator op, bit Reverse, bits<3> cc> {
  def : Pat<(op RC:$rA, RC:$rB),
            !if(Reverse, (CMPr $rB, $rA, cc),
                         (CMPr $rA, $rB, cc))>;
  def : Pat<(op RC:$rA, ImmSExt6:$rB),
            !if(Reverse, (CMPi $rB, $rA, cc),
                         (CMPi $rA, $rB, cc))>;
}

class NemesysCCs {
  bits<3> EQ  = 0b000;
  bits<3> NE  = 0b001;
  bits<3> LT  = 0b010;
  bits<3> LE  = 0b011;
  bits<3> ULT = 0b100;
  bits<3> ULE = 0b101;
}

def CC : NemesysCCs;

defm : CMPPat<seteq,  0, CC.EQ>;
defm : CMPPat<setne,  0, CC.NE>;
defm : CMPPat<setlt,  0, CC.LT>;
defm : CMPPat<setle,  0, CC.LE>;
defm : CMPPat<setult, 0, CC.ULT>;
defm : CMPPat<setule, 0, CC.ULE>;

defm : CMPPat<setge,  1, CC.LT>;
defm : CMPPat<setgt,  1, CC.LE>;
defm : CMPPat<setuge, 1, CC.ULT>;
defm : CMPPat<setugt, 1, CC.ULE>;

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

def BR : NemesysInst<0x13, (outs), (ins PC:$pA, brtarget:$tgt),
                     "br $pA, $tgt", []> {
  bits<3> pA;
  bits<16> tgt;
  let isBranch = 1;
  // TODO: encode predicate inversion
  //let Inst{19} = inv;
  let Inst{18 - 16} = pA;
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
