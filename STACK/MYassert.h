#include <stdio.h>
#include "colors.h"

#define DOTXT(Message) #Message

#define MCP(Message)                                                                                       \
        do                                                                                                 \
        {                                                                                                  \
            fprintf (stderr, "Failed check " KRED Kbright DOTXT (Message) " "                              \
            KNRM "in " KYLW "%s" KNRM " in " KWHT Kunderscore "%s:%d\n" KNRM,                                                              \
             __PRETTY_FUNCTION__, __FILE__, __LINE__ );                                                     \
        } while (0)


#define MCA(Condition, ReturnNum)                     \
        do                                            \
        {                                             \
            if (!(Condition))                         \
            {                                         \
                MCP (Condition);                      \
                return ReturnNum;                     \
            }                                         \
        } while (0)
