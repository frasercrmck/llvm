class NemesysReg<string n, bits<16> enc, list<string> altNames = []>
      : Register<n, altNames> {
  let HWEncoding = enc;
  let Namespace = "Nemesys";
}

foreach i = 0-31 in {
  def R#i : NemesysReg<"r"#i, i>;
}

def RC : RegisterClass<"Nemesys", [i32], 32, (add (sequence "R%i", 0, 31))> {
  let DiagnosticString = "operand must be a register in range [r0, r31]";
}

foreach i = 0-7 in {
  def P#i : NemesysReg<"p"#i, i>;
}

def PC : RegisterClass<"Nemesys", [i1], 32, (add (sequence "P%i", 0, 7))> {
  let DiagnosticString = "operand must be a predicate in range [p0, p7]";
}
