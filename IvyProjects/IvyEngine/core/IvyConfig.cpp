///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include "IvyConfig.h"


///@TODO Debug Includes
#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyConfigGetItem
///
/// @brief
///     
/// @return
///     
///////////////////////////////////////////////////////////////////////////////////////////////////
IvyConfigItem* IvyConfigGetItem(IvyConfigItem* pItemList, const char* name)
{
    IvyConfigItem* pConfigItem = NULL;

    unsigned int count = 0;
    while (pItemList[count].name != NULL)
    {
        if (stricmp(name, pItemList[count].name) == 0)
        {
            pConfigItem = &pItemList[count];
            break;
        }

        count++;
    }

    return pConfigItem;
}


///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyConfigGetName
///
/// @brief
///     
/// @return
///     Return NULL if GetName didn't find name, endPos=0 if it found invalid input
///////////////////////////////////////////////////////////////////////////////////////////////////
char* IvyConfigGetName(
    const char* argString,  ///<
    unsigned int startPos,  ///<
    char* nameToken,        ///<
    unsigned int* endPos)   ///< [out]
{
    // Establish starting condition that assumes error.  retPtr = NULL & endPos = 0
    char* retPtr = NULL;
    *endPos = 0;

    // Skip past initial whitespace
    while (argString[startPos] == ' ')
    {
        startPos++;
    }

    if (argString[startPos] == '\0')
    {
        *endPos = startPos;
    }
    else
    {
        // if equal sign present, get left side
        // else get token delimited by space
        // AKA get token delimited by space or '='
        const char *delimEqual = strchr(&argString[startPos], '=');
        const char *delimSpace = strchr(&argString[startPos], ' ');
        const char *delim = NULL;

        if ((delimEqual != NULL) && (delimEqual < delimSpace))
        {
            delim = delimEqual;
        }
        else
        {
            delim = delimSpace;
        }

        if (delim != NULL)
        {
            unsigned int length = delim - &argString[startPos];

            if (length != 0)
            {
                strncpy(nameToken, &argString[startPos], length);
                *endPos = startPos + length;
                retPtr = nameToken;
            }
        }
    }

    return retPtr;
}

///////////////////////////////////////////////////////////////////////////////////////////////////
/// IvyConfigGetValue
///
/// @brief
///     
/// @return
///     
///////////////////////////////////////////////////////////////////////////////////////////////////
char* IvyConfigGetValue(
    const char* argString,  ///<
    unsigned int startPos,  ///< 
    char* valueToken,       ///< [out]
    unsigned int* endPos)   ///< [out]
{
    // Establish starting condition that assumes error.  retPtr = NULL & endPos = 0
    char* retPtr = NULL;
    *endPos = 0;

    ///@TODO if string, verify first and last quote

    // Skip past '=' and inner whitespace
    while ((argString[startPos] == ' ') || (argString[startPos] == '='))
    {
        startPos++;
    }

    const char *delim = strchr(&argString[startPos], ' ');

    if (delim != NULL)
    {
        unsigned int length = (unsigned int)(delim - &argString[startPos]);

        if (length != 0)
        {
            strncpy(valueToken, &argString[startPos], length);
            *endPos = startPos + length;
            retPtr = valueToken;
        }
    }

    return retPtr;
}


const char* IvyConfigBuildString(unsigned int argc, const char** argv)
{
    // Calculate Argument String Length
    unsigned int argBufferLength = 0;
    for (unsigned int idx = 1; idx < argc; ++idx)
    {
        argBufferLength += strlen(argv[idx]) + 1; // +1 for space delimiter
    }

    // Build Argument String
    char* argBuffer = new char[argBufferLength+1];
    memset(argBuffer, 0, argBufferLength+1);

    unsigned int argBufferProgress = 0;
    for (unsigned int idx = 1; idx < argc; ++idx)
    {
        strcpy(&argBuffer[argBufferProgress], argv[idx]);
        argBufferProgress += strlen(argv[idx]);

        // add space delimiter
        argBuffer[argBufferProgress] = ' ';
        argBufferProgress++;
    }

    return argBuffer;
}



bool IvyConfigParseConfigString(const char* configString, IvyConfigItem* pItemList)
{

    // Parse argument string
    bool argError = false;
    char* nameTokenBuffer = new char[1024];
    char* valueTokenBuffer = new char[1024];

    unsigned int startPos = 0;
    unsigned int endPos = 0;

    while(!argError)
    {
        memset(nameTokenBuffer, 0, 1024);
        memset(valueTokenBuffer, 0, 1024);

        if (IvyConfigGetName(configString, startPos, nameTokenBuffer, &endPos))
        {
            startPos = endPos;

            IvyConfigItem* pConfigItem = IvyConfigGetItem(pItemList, nameTokenBuffer);

            if (pConfigItem == NULL)
            {
                argError = true;
            }
            else
            {
                if (pConfigItem->type != IvyBool)
                {
                    if (IvyConfigGetValue(configString, startPos, valueTokenBuffer, &endPos))
                    {
                        startPos = endPos;
                    }
                    else
                    {
                        argError = true;
                    }
                }

                std::cout << "Name: " << nameTokenBuffer << "  Value: " << valueTokenBuffer << std::endl;
            }
        }
        else
        {
            argError = (endPos == 0);
            break;
        }

        // process name,value pair

        /*
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
        */
    }

    if (argError)
    {
        std::cout << "ERROR ERROR ERROR" << std::endl;
    }

    return !argError;
}

