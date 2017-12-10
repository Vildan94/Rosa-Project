#ifndef rosa_EXT_H_
#define rosa_EXT_H_

#include "rosa_def.h"

extern void contextInit(tcb * tcbTask);
extern void contextRestore(void);
extern void contextSave(void);
extern void contextSaveFromISR(void);
extern void contextRestoreFromISR(void);

#endif /* rosa_EXT_H_ */
