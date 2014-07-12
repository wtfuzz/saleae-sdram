#ifndef _SDRAM_COMMANDS_H_
#define _SDRAM_COMMANDS_H_

typedef enum _sdram_commands_t
{
  NOP=0x7,
  BST=0x6,
  READ=0x5,
  WRITE=0x4,
  ACT=0x3,
  PRE=0x2,
  REF=0x1,
  MRS=0x0
} sdram_commands_t;

#endif
