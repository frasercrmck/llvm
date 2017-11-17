include "NemesysInstrFormats.td"

def ImmSExt6AsmOperand : AsmOperandClass {
  let Name = "ImmSExt6";
  let RenderMethod = "addImmOperands";
  let DiagnosticString = "operand must be 6-bit signed immediate";
}

def immSExt6 : Operand<i32>,
               ImmLeaf<i32, [{ return isInt<6>(Imm); }]> {
  let ParserMatchClass = ImmSExt6AsmOperand;
}

multiclass ALUInst<string mnemonic, SDNode node> {
  def r : NemesysInst<(outs RC:$rZ), (ins RC:$rA, RC:$rB),
                       !strconcat(mnemonic, " $rA, $rB, $rZ"),
                       [(set RC:$rZ, (node RC:$rA, RC:$rB))]>;

  def i : NemesysInst<(outs RC:$rZ), (ins RC:$rA, immSExt6:$rB),
                       !strconcat(mnemonic, " $rA, $rB, $rZ"),
                       [(set RC:$rZ, (node RC:$rA, immSExt6:$rB))]>;
}

defm ADD : ALUInst<"add", add>;
