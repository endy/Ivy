///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     App Config Sandbox
///
///     Copyright 2011-2012, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>


/*
Precidence:
1. Command line  - last read value
2. Config file - last read value.
3. App defaults

Config file: name=value pairs.
Will ignore comments that take the form of // or # as first character on the line

Any bad command will be thrown into log file as an error and application exits

All values must have valid defaults at app-start time.
Settings without valid defaults are required.

Settings can be required, without valid value program will end.

any settings can be in config file or overridden in command line

type initial prefixes all names by convention, not required.

Any 'required' fields that the user is expected to input will require a temporary application to receive said input
else will fallback on STDIN.

Expected Ivy Required Fields but with defaults
Window Width / Height
DX or GL.. default is 



GetName / GetValue
return non-null if got token, null else.  start = end if no more tokens

startpos = endpos if found nothing


*/

const char* ConsoleHeader = "\
Ivy Engine - Version 0.0001                 \n\
Copyright 2011, 2012 - Brandon Light              \n\n";

enum ArgType
{
    IvyUnknown = 0,
    IvyInt,
    IvyUint,
    IvyFloat,
    IvyBool,
    IvyChar,
    IvyString,
};

struct ArgData
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
        const char*     pcValue;
        const char**    ppStrValue;
    } value;
    //*/

    bool required;
};

// 
unsigned int screenWidth = 640;
unsigned int screenHeight = 480;
bool useGL = true;
bool useD3D = false;

char buttonX = 'x';
char buttonY = 'y';
float scale = 0.0f;
int offset = 0;
char* testString = "DEFAULT SETTING";

ArgData inputArgs[] = 
{
    { "uScreenWidth",   "Screen Width",                 IvyUint,    &screenWidth,   false },
    { "uScreenHeight",  "Screen Height",                IvyUint,    &screenHeight,  false },
    { "bUseGL",         "Use OpenGL",                   IvyBool,    &useGL,         false },
    { "bUseD3D",        "Use Direct3D",                 IvyBool,    &useD3D,        false },

    // test params
    { "cButtonX", "X Button", IvyChar, &buttonX, false },
    { "cButtonY", "Y Button", IvyChar, &buttonY, false },
    { "fScale", "Generic Scale", IvyFloat, &scale, false },
    { "iOffset", "Generic Offset", IvyInt, &offset, false },
    { "sPrintValue",    "Print the argument to output", IvyString,  &testString,   false },

    // Sentinel
    { NULL, NULL, IvyUnknown, NULL, false }
};

/*
i,u,f,b,c,s
uPrintNumber=3
uPrintValue="blah"
uScreenWidth=512 uScreenHeight=512 bUseGL bUseD3D
*/

ArgData* GetArgData(const char* name)
{
    ArgData* pArgData = NULL;

    unsigned int count = 0;
    while (inputArgs[count].argName != NULL)
    {
        if (stricmp(name, inputArgs[count].argName) == 0)
        {
            pArgData = &inputArgs[count];
            break;
        }

        count++;
    }

    return pArgData;
}

// Return NULL if didnt find name, endPos=0 if it found invalid input
char* GetName(
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

char* GetValue(
    char* argString,        ///<
    unsigned int startPos,  ///< 
    char* valueToken,       ///< [out]
    unsigned int* endPos)   ///< [out]
{
    // Establish starting condition that assumes error.  retPtr = NULL & endPos = 0
    char* retPtr = NULL;
    *endPos = 0;

    ///@todo if string, verify first and last quote

    // Skip past '=' and inner whitespace
    while ((argString[startPos] == ' ') || (argString[startPos] == '='))
    {
        startPos++;
    }

    char *delim = strchr(&argString[startPos], ' ');

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

int main(unsigned int argc, char** argv)
{
    std::cout << ConsoleHeader;

    // Print command line arguments
    for (unsigned int idx = 0; idx < argc; ++idx)
    {
        std::cout << idx << " " << argv[idx] << std::endl;
    }

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

        if (GetName(argBuffer, startPos, nameTokenBuffer, &endPos))
        {
            startPos = endPos;

            ArgData* pArgData = GetArgData(nameTokenBuffer);

            if (pArgData == NULL)
            {
                argError = true;
            }
            else
            {
                if (pArgData->type != IvyBool)
                {
                    if (GetValue(argBuffer, startPos, valueTokenBuffer, &endPos))
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

    std::cout << "Print String = " << testString << std::endl;

    return 0;
}
