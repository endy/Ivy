///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     App Config Sandbox
///
///     Copyright 2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>

const char* ConsoleHeader = "\
Ivy Engine - Version 0.0001                 \n\
Copyright 2011 - Brandon Light              \n\n";

enum ArgType
{
    IvyUnknown = 0,
    IvyInt,
    IvyUint,
    IvyFloat,
    IvyBool,
    IvyString,
};

typedef union _ArgDataValue
{
    int          iValue;
    unsigned int uValue;
    float        fValue;
    bool         bValue;
    char*        strValue;
} ArgDataValue;

struct ArgDataInfo
{
    const char* argName;
    const char* argDesc;
    ArgType type;

    //*
    union 
    {
        void*           pvValue;
        unsigned int*   puValue;
        int*            piValue;
        float*          pfValue;
        bool*           pbValue;
        const char**    ppStrValue;
    } value;
    //*/

    bool required;
};

const char* printString = "DEFAULT SETTING";
unsigned int intValue = 0;

ArgDataInfo inputArgs[] = 
{
    { "-printNumber", "Number to print",                IvyUint,    &intValue,      false },
    { "-printValue", "Print the argument to output",    IvyString,  &printString,   false }
};


int main(unsigned int argc, const char** argv)
{
    std::cout << ConsoleHeader;

    for (unsigned int idx = 0; idx < argc; ++idx)
    {
        std::cout << idx << " " << argv[idx] << std::endl;
    }

    int count = 1;
    while(count < argc)
    {
        for (int idx = 0; idx < 2; ++idx)
        {
            if (stricmp(argv[count], inputArgs[idx].argName) == 0)
            {
                if (inputArgs[idx].type == IvyUint)
                {
                    sscanf(argv[count+1], "%u", inputArgs[idx].value.puValue);
                }
                else if (inputArgs[idx].type == IvyString)
                {
                    *inputArgs[idx].value.ppStrValue = argv[count+1];
                }

                count++;
            }
        }

        count++;
    }

    std::cout << "Print String = " << printString << std::endl;
    std::cout << "Int Value = " << intValue << std::endl;

    return 0;
}
