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

def MOV : NemesysInst<(outs RC:$rZ), (ins ImmSExt16:$rA),
                       "mov $rA, $rZ",
                      [(set RC:$rZ, ImmSExt16:$rA)]>;
def MOVH : NemesysInst<(outs RC:$rZ), (ins ImmSExt16:$rA),
                       "movh $rA, $rZ", []>;
