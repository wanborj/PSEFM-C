#ifndef __PSEFMCONFIGURE_H
#define __PSEFMCONFIGURE_H

#define id_t int
#define tick_t long
#define bool int

#define NUMOFMODES 2  // the maximal number of modes in system, the id of mode start from 0
#define NUMOFCONDS 4 // the maximal number of mode switch condition in system
#define NUMOFTASKS 4 // the maximal number of tasks in one mode
#define NUMOFSERVANTS 6 // the maximal number of servants in one task

typedef enum {SEN=1, CONT, ACT} servant_t;

#endif
