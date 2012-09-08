///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _IVYCONFIG_H_
#define _IVYCONFIG_H_

#include "IvyTypes.h"

enum IvyConfigItemType
{
    IvyUnknown  = 0,
    IvyInt,
    IvyUint,
    IvyFloat,
    IvyBool,
    IvyChar,
    IvyString,
};

struct IvyConfigItem
{
    const char*         name;
    const char*         desc;
    IvyConfigItemType   type;

    union
    {
        void*           pvValue;
        unsigned int*   puValue;
        int*            piValue;
        float*          pfValue;
        bool*           pbValue;
        const char*     pcValue;
        const char**    ppStrValue;
    } value;

    bool                required;
};

char* IvyConfigBuildString(unsigned int argc, const char** argv);

bool IvyConfigParseConfigString(const char* configString, IvyConfigItem* pItemList);

#endif // _IVYCONFIG_H_

