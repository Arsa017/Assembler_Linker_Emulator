# file: main.s

.extern mathAdd, mathSub, mathMul, mathDiv, mathOpr

.global my_start

.global value0, value1, value2, value3, value4, value5, value6

.section .myCode
my_start:
  ldr r6, $0xFEFE # init SP

  wait:
  ldr r0, myCounter
  ldr r2, $1
  add r0, r2
  str r0, myCounter
  ldr r1, $12
  push r0
  push r1


  ldr r0, $4 # ivt entry number for isr_user0 
  int r0

  ldr r0, $5
  push r0
  ldr r0, $4
  push r0
  ldr r0, $destinations
  call *[r0] # pc <= mathOpr
  str r0, value8

  pop r2      # skidamo 2 vrednosti sa steka koje se odnose na push r0
  pop r3      # namenjenih za izracunavanje matematickih izraza

  ldr r0, $0
  push r0
  ldr r0, $1
  push r0
  call mathAdd # pc <= mathAdd
  str r0, value1

  pop r2      # skidamo 2 vrednosti sa steka koje se odnose na push r0
  pop r3      # namenjenih za izracunavanje matematickih izraza

  ldr r0, $1
  push r0
  ldr r0, $1
  push r0
  call %mathAdd # pc <= pc + (mathAdd - pc) ~ mathAdd
  str r0, value2

  pop r2      # skidamo 2 vrednosti sa steka koje se odnose na push r0
  pop r3      # namenjenih za izracunavanje matematickih izraza

  ldr r0, $8
  push r0
  ldr r0, $11
  push r0
  ldr r0, $4
  ldr r1, $destinations
  add r0, r1
  call *[r0] # pc <= mem16[r0] ~ mem16[2 + destinations] ~ mathSub
  str r0, value3

  pop r2      # skidamo 2 vrednosti sa steka koje se odnose na push r0
  pop r3      # namenjenih za izracunavanje matematickih izraza

  ldr r0, $2
  push r0
  ldr r0, $2
  push r0
  ldr r0, $6
  call *[r0 + destinations] # pc <= mem16[r0 + destinations] ~ mathMul
  str r0, value4

  pop r2      # skidamo 2 vrednosti sa steka koje se odnose na push r0
  pop r3      # namenjenih za izracunavanje matematickih izraza

  ldr r0, $5
  push r0
  ldr r0, $25
  push r0
  ldr r0, $8
  ldr r1, $destinations
  add r0, r1
  ldr r0, [r0]
  call *r0 # pc <= r0 ~ mem16[6 + destinations] ~ mathDiv
  str r0, value5

  pop r2      # skidamo 2 vrednosti sa steka koje se odnose na push r0
  pop r3      # namenjenih za izracunavanje matematickih izraza

  pop r1
  pop r0
  cmp r1, r0
  jgt wait
  str r0, value7

  ldr r0, value0
  ldr r1, value8
  ldr r2, value2
  ldr r3, value3
  ldr r4, value4
  ldr r5, value5
  ldr r6, value7

  halt

.section .myData
value0:
.word 0
value1:
.word 0
value2:
.word 0
value3:
.word 0
value4:
.word 0
value5:
.word 0
value6:
.word 0
value7:
.word 0
value8:
.word 0
myCounter:
.word 0
destinations:
.word mathOpr
.word mathAdd
.word mathSub
.word mathMul
.word mathDiv


.end
