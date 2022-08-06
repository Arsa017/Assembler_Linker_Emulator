# Single_pass_assembler

Single pass assembler

Single pass assembler for creating .o files from .s files Symbol table

#Specification

- one line of source code contains at most one assembler command / directive
- the label, which ends with a colon, can be found at the beginning of the source code line after any number of white characters-
- the label can stand independently, without the accompanying assembly command / directive in the same line of the original code, which is equivalent to standing in line   with the source code with the first following content
- symbols are exported by specifying the assembly directive .global <symbol_list> where in within one directive it is possible to specify several symbols separated by     commas
- symbols are imported by specifying the assembly directive .extern <symbol_list> where in within one directive it is possible to specify several symbols separated by commas
- source code representing assembly commands and assembly directives for generating content must be within the sections defined by the assembly directive .section         <section_name>:
- the source code file is terminated (the rest of the file is discarded, ie not translated) using assembler directive .end
  space allocation is done using assembly directives .byte <symbol_list / literal>, .word <symbol_literal> and .skip whose functionality matches the GNU assembler         assembly directives of the same name

# Linker

 - Linker is used to link an object files from input, that are obtained as a result of assembly, and to make a signle hex file which can be executed by emulator

# Emulator

- Implementation of Emulator as an infinite while loop for reading a ready hex file from input and executing instructions util the reach of halt instruction
- Updating flags of abstract processor and setting the correct values into memory for every executed instruction
