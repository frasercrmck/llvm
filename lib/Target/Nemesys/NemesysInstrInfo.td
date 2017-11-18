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

multiclass ALUInst<string mnemonic, SDNode node> {
  def r : NemesysInst<(outs RC:$rZ), (ins RC:$rA, RC:$rB),
                       !strconcat(mnemonic, " $rA, $rB, $rZ"),
                       [(set RC:$rZ, (node RC:$rA, RC:$rB))]>;

  def i : NemesysInst<(outs RC:$rZ), (ins RC:$rA, ImmSExt6:$rB),
                       !strconcat(mnemonic, " $rA, $rB, $rZ"),
                       [(set RC:$rZ, (node RC:$rA, ImmSExt6:$rB))]>;
}

defm ADD : ALUInst<"add", add>;
defm SUB : ALUInst<"sub", sub>;
defm MPY : ALUInst<"mpy", mul>;

defm AND : ALUInst<"and", and>;
defm OR  : ALUInst<"or",  or>;
defm XOR : ALUInst<"xor", xor>;

defm SHL : ALUInst<"shl", shl>;
defm SRA : ALUInst<"sra", sra>;
defm SRL : ALUInst<"srl", srl>;

def CondCodeAsmOperand : AsmOperandClass {
  let Name = "CondCode";
  let RenderMethod = "addCondCodeOperands";
  let DiagnosticString = "operand must be a condition code";
}

def CondCode : Operand<i32>, ImmLeaf<i32, "return isInt<"#b#">(Imm);"> {
  let PrintMethod = "printCondCode";
  let ParserMatchClass = CondCodeAsmOperand;
}

def CMPr : NemesysInst<(outs PC:$pZ),
                       (ins RC:$rA, RC:$rB, CondCode:$cc),
                       "cmp.$cc $rA, $rB, $pZ", []>;
def CMPi : NemesysInst<(outs PC:$pZ),
                       (ins RC:$rA, ImmSExt6:$rB, CondCode:$cc),
                       "cmp.$cc $rA, $rB, $pZ", []>;

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

def MOV : NemesysInst<(outs RC:$rZ), (ins ImmSExt16:$rA),
                       "mov $rA, $rZ",
                      [(set RC:$rZ, ImmSExt16:$rA)]>;
def MOVH : NemesysInst<(outs RC:$rZ), (ins ImmSExt16:$rA),
                       "movh $rA, $rZ", []>;

def PCRel16AsmOperand : AsmOperandClass {
  let Name = "PCRel16";
  let PredicateMethod = "isImm";
  let RenderMethod = "addImmOperands";
  let ParserMethod = "parsePCRel16";
}

def brtarget : Operand<OtherVT> {
  let ParserMatchClass = PCRel16AsmOperand;
}

def BR : NemesysInst<(outs), (ins PC:$pA, brtarget:$tgt),
                     "br $pA, $tgt", []> {
  let isBranch = 1;
}

def CALL : NemesysInst<(outs), (ins brtarget:$tgt),
                       "call $tgt", []> {
  let isCall = 1;
}

def RET : NemesysInst<(outs), (ins), "ret", []> {
  let isReturn = 1;
  let isBarrier = 1;
  let isTerminator = 1;
}

def HALT : NemesysInst<(outs), (ins), "halt", []> {
  let hasSideEffects = 1;
}
