//
// Created by zpc on 18-12-31.
//

#include "int2char.h"

void int2char(int id, char *name)
{
    int i=3;
    while(i)
    {
        i--;
        name[i] = id % 10 + '0';
        id = id/10;
    }
    name[3]='\0';
    return;
}