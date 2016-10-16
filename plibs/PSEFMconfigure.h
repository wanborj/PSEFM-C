#ifndef __PSEFMconfigure_H
#define __PSEFMconfigure_H

#define id_type int
#define bool int

#define NUMOFMODES 10  // the maximal number of modes in system, the id of mode start from 0
#define NUMOFCONDS 10 // the maximal number of mode switch condition in system
#define NUMOFTASKS 10 // the maximal number of tasks in one mode

extern ps_mode_cond switch_cond[NUMOFCONDS];
extern ps_mode_t modes[NUMOFMODES];


#endif
