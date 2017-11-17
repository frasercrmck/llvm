class NemesysReg<string n, list<string> altNames = []>
      : Register<n, altNames> {
  field bits<5> Num;
  let Namespace = "Nemesys";
}

foreach i = 0-31 in {
  def R#i : NemesysReg<"r"#i>;
}

def RC : RegisterClass<"Nemesys", [i32], 32, (add (sequence "R%i", 0, 31))> {
  let DiagnosticString = "operand must be a register in range [r0, r31]";
}
