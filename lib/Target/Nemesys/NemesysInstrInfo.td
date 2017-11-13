include "NemesysInstrFormats.td"

def ADD : NemesysInst<(outs RC:$rZ), (ins RC:$rA, RC:$rB),
                      "add $rA, $rB, $rZ",
                      [(set RC:$rZ, (add RC:$rA, RC:$rB))]>;
