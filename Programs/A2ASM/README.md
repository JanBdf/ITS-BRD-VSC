# README for task A2

## Description of program functionality

Anw01 

|       |                                            |
|-------|--------------------------------------------|
| Anw01 | Load address of VariableA into register R0 |
| Anw02 | Load value of byte at R0 (LSB (0xEF)) into register R2  |
| Anw03 | Load value of byte at R0 + 1 (MSB (0xBE)) into register R3 |
| Anw04 | Shift all bits in R2 to the left 8 places (LSB -> MSB) |
| Anw05 | For every bit, store a 1 in R2 if bit is 1 in R2 or R3. This will put the value of R3 into the LSB of R2 |
| Anw06 | Store result into VariableA. MSB and LSB are now switched |
| Anw07 | Load value of ConstByteA into R5 |
| Anw08 | Store value of R5 at address in R0 (VariableA) |
| Anw09 | Load address of VariableB into R1 |
| Anw0A | Load value at R1 (value of VariableB) into R6 |
| Anw0B | Load constant value 0x30ED into register R7 |
| Anw0C | Add values of R6 and R7 and store result in R6 |
| Anw0D | Store value of R6 at address in R1 (VariableB) |
| Anw0E | Branches to its own address, looping indefinitely |
