///////////////////////////////////////////////////////////////////////////////////////////////////
///
///     Ivy Engine
///
///     Copyright 2010-2011, Brandon Light
///     All rights reserved.
///
///////////////////////////////////////////////////////////////////////////////////////////////////

///@todo Remove _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1

#include "IvyTypes.h"
#include "IvyMemory.h"
#include "IvyImporter.h"

#include <Windows.h>

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include <vector>
#include <algorithm>

using std::ifstream;
using std::iostream;
using std::stringstream;
using std::string;

///@todo REFACTOR THIS HEADER OUT
#include <xnamath.h>
///@todo_end

///@todo REMOVE THIS TEST

#include <cstdio>


#include "IvyPerf.h"

inline bool mygetline(
    std::vector<char>::iterator& beginPos,
    std::vector<char>& fileBuffer,
    string& line)
{
    bool valid = false;
    if (beginPos != fileBuffer.end())
    {
        std::vector<char>::iterator endPos =
            std::find(beginPos, fileBuffer.end(), '\n');
        line = string(beginPos, endPos);
        beginPos = ++endPos;
        valid = true;
    }

    return valid;
}

inline void tokenize(
    string& s,
    std::vector<string>& tokens,
    string delim)
{
    int i = 0;

    // skip delimiters at beginning.
    string::size_type lastPos = s.find_first_not_of(delim, 0);

    // find first "non-delimiter".
    string::size_type pos = s.find_first_of(delim, lastPos);

    while (string::npos != pos || string::npos != lastPos)
    {
        // found a token, add it to the vector.
        tokens[i++] = string(s.substr(lastPos, pos - lastPos));

        // skip delimiters.  Note the "not_of"
        lastPos = s.find_first_not_of(delim, pos);

        // find next "non-delimiter"
        pos = s.find_first_of(delim, lastPos);
    }
}

void ImportFstream(
    const CHAR* pPlyFilename,
    UINT& numVertices,
    VertexPTN**& ppVB,
    UINT& numIndices,
    UINT**& ppIB)
{
    PerfData perf;
    DOUBLE perfoverall = 0, perfheader = 0, perfverts = 0, perffaces = 0;
    
    ifstream fs;
    string line;
    UINT vertices = 0;
    UINT faces = 0;

    const bool UseStringStream = FALSE;

    perf.push(); // overall

    // open file and get the size of it
    fs.open(pPlyFilename, std::ios::in | std::ios::ate);
    std::streampos fileSize = fs.tellg();
    fs.seekg(std::ios::beg);
    

    // read file into char vector
    std::vector<char> fileBuffer(fileSize);
    fs.read(&fileBuffer[0], fileSize);

    perf.push(); // header
 
    std::vector<char>::iterator beginPos = fileBuffer.begin();
    std::vector<char>::iterator endPos = beginPos;

    // capture header
    while (mygetline(beginPos, fileBuffer, line))
    {
        if (line.compare("end_header") == 0)
        {
            break;
        }
        else
        {
            std::vector<string> tokens(10, "");
            tokenize(line, tokens, " ");

            if (tokens[0].compare("element") == 0)
            {
                if (tokens[1].compare("vertex") == 0)
                {
                    stringstream t(tokens[2]);
                    t >> vertices;
                }
                else if (tokens[1].compare("face") == 0)
                {
                    stringstream t(tokens[2]);
                    t >> faces;
                }
            }
            else if (tokens[0].compare("property") == 0)
            {

            }
        }
    }

    perfheader = perf.pop();    // header

    numVertices = vertices;
    VertexPTN* pVerts = IvyTypeAlloc(VertexPTN, numVertices);
    memset(pVerts, 0, sizeof(pVerts));

    perf.push(); // verts


    std::vector<string> tokens(10, "");
    for (UINT vIdx = 0; vIdx < vertices; ++vIdx) 
    {
        if (mygetline(beginPos, fileBuffer, line))
        {
            tokenize(line, tokens, " ");
            
            if (UseStringStream)
            {
                //*
                stringstream(tokens[0]) >> pVerts[vIdx].Pos.x;
                stringstream(tokens[1]) >> pVerts[vIdx].Pos.y;
                stringstream(tokens[2]) >> pVerts[vIdx].Pos.z;
                //*/
            }
            else
            {
                sscanf(tokens[0].c_str(), "%f", &pVerts[vIdx].Pos.x);
                sscanf(tokens[1].c_str(), "%f", &pVerts[vIdx].Pos.y);
                sscanf(tokens[2].c_str(), "%f", &pVerts[vIdx].Pos.z);
            }

            // confidence not used
            // std::getline(ssline, token, ' ');

            // intensity not used
            // std::getline(ssline, token, ' ');

            // error handlign for correct number of properties per element
        }
        else
        {
            std::cerr << "Incorrect number of verts" << std::endl; 
        }
    }

    perfverts = perf.pop();

    numIndices = faces*3;
    UINT* pIB = IvyTypeAlloc(UINT, numIndices);

    perf.push(); // faces

    for (UINT fIdx = 0; fIdx < faces; ++fIdx) 
    {
        if (mygetline(beginPos, fileBuffer, line))
        {
            tokenize(line, tokens, " ");

            if (UseStringStream)
            {
                //*
                stringstream(tokens[1]) >> pIB[fIdx*3 + 0];
                stringstream(tokens[2]) >> pIB[fIdx*3 + 1];
                stringstream(tokens[3]) >> pIB[fIdx*3 + 2];
                //*/
            }
            else
            {
                sscanf(tokens[1].c_str(), "%d", &pIB[fIdx*3 + 0]);
                sscanf(tokens[2].c_str(), "%d", &pIB[fIdx*3 + 1]);
                sscanf(tokens[3].c_str(), "%d", &pIB[fIdx*3 + 2]);
            }

            VertexPTN& v0 = pVerts[pIB[fIdx*3+0]];
            VertexPTN& v1 = pVerts[pIB[fIdx*3+1]];
            VertexPTN& v2 = pVerts[pIB[fIdx*3+2]];

            FXMVECTOR xv0 = { v0.Pos.x, v0.Pos.y, v0.Pos.z};
            FXMVECTOR xv1 = { v1.Pos.x, v1.Pos.y, v1.Pos.z };
            FXMVECTOR xv2 = { v2.Pos.x, v2.Pos.y, v2.Pos.z };

            FXMVECTOR xv01 = XMVector3Normalize(XMVectorSubtract(xv0, xv1));
            FXMVECTOR xv12 = XMVector3Normalize(XMVectorSubtract(xv1, xv2));


            FXMVECTOR norm = XMVector3Cross(xv01, xv12);

            Point4 p = { norm.m128_f32[0], norm.m128_f32[1], norm.m128_f32[2], 0 };
            v0.N = p;
            v1.N = p;
            v2.N = p;


            // error handlign for correct number of properties per element
        }
        else
        {
            std::cerr << "Incorrect number of faces" << std::endl; 
        }
    }
    
    perffaces = perf.pop();
    perfoverall = perf.pop();
    
    fs.close();

    *ppVB = pVerts;
    *ppIB = pIB;
    
    //*/

    std::cout << "HEADER: " << perfheader << " s" << std::endl;
    std::cout << "VERTS: " << perfverts << " s" << std::endl;
    std::cout << "FACES: " << perffaces << " s" << std::endl;
    std::cout << "OVERALL: " << perfoverall << " s" << std::endl;
}

void ImportCString(
    const CHAR* pPlyFilename,
    UINT& numVertices,
    VertexPTN**& ppVB,
    UINT& numIndices,
    UINT**& ppIB)
{
    PerfData perf;

    DOUBLE perfoverall = 0, perfheader = 0, perfverts = 0, perffaces = 0;

    FILE* pFile = NULL;
    char* line = NULL;

    UINT vertices = 0;
    UINT faces = 0;

    pFile = fopen(pPlyFilename, "r");

    if (pFile == NULL)
    {
        return; // bad filename
    }

    const unsigned int BufferSize = 1024;
    char buffer[BufferSize];

    perf.push(); // overall
    perf.push(); // header

    while (fgets(buffer, BufferSize, pFile))
    {
        char* token = strtok(buffer, " ");

        if (strncmp("end_header", token, 10) == 0)
        {
            break;
        }
        else
        {         
            if (stricmp("element", token) == 0)
            {
                token = strtok(NULL, " ");
                if (stricmp("vertex", token) == 0)
                {
                    token = strtok(NULL, " ");
                    sscanf(token, "%u", &vertices);
                }
                else if (stricmp("face", token) == 0)
                {
                    token = strtok(NULL, " ");
                    sscanf(token, "%u", &faces);
                }
            }
            else if (stricmp("property", token) == 0)
            {
                // do nothing currently
            }
        }       
    }

    perfheader = perf.pop(); 	// header

    numVertices = vertices;
    VertexPTN* pVerts = IvyTypeAlloc(VertexPTN, numVertices);
    memset(pVerts, 0, sizeof(pVerts)*vertices);

    perf.push(); // verts

    for (UINT vIdx = 0; vIdx < vertices; ++vIdx) 
    {
        if (fgets(buffer, BufferSize, pFile))
        {
            char* token = strtok(buffer, " ");
            sscanf(token, "%f", &pVerts[vIdx].Pos.x);
            token = strtok(NULL, " ");
            sscanf(token, "%f", &pVerts[vIdx].Pos.y);
            token = strtok(NULL, " ");
            sscanf(token, "%f", &pVerts[vIdx].Pos.z);
        }
        else
        {
            // error
            //return;
        }
    }

    perfverts = perf.pop();

    numIndices = faces*3;
    UINT* pIB = IvyTypeAlloc(UINT, numIndices);

    perf.push(); // faces


    for (UINT fIdx = 0; fIdx < faces; ++fIdx) 
    {
        if (fgets(buffer, BufferSize, pFile))
        {
            char* token = strtok(buffer, " ");

            UINT indexCount = 0;
            sscanf(token, "%u", &indexCount);

            // assert index count = 3, else error because we assume 3 for now

            token = strtok(NULL, " ");
            sscanf(token, "%u", &pIB[fIdx*3 + 0]);
            token = strtok(NULL, " ");
            sscanf(token, "%u", &pIB[fIdx*3 + 1]);
            token = strtok(NULL, " ");
            sscanf(token, "%u", &pIB[fIdx*3 + 2]);

            VertexPTN& v0 = pVerts[pIB[fIdx*3+0]];
            VertexPTN& v1 = pVerts[pIB[fIdx*3+1]];
            VertexPTN& v2 = pVerts[pIB[fIdx*3+2]];

            FXMVECTOR xv0 = { v0.Pos.x, v0.Pos.y, v0.Pos.z};
            FXMVECTOR xv1 = { v1.Pos.x, v1.Pos.y, v1.Pos.z };
            FXMVECTOR xv2 = { v2.Pos.x, v2.Pos.y, v2.Pos.z };

            FXMVECTOR xv01 = XMVector3Normalize(XMVectorSubtract(xv0, xv1));
            FXMVECTOR xv12 = XMVector3Normalize(XMVectorSubtract(xv1, xv2));

            FXMVECTOR norm = XMVector3Cross(xv01, xv12);

            Point4 p = { norm.m128_f32[0], norm.m128_f32[1], norm.m128_f32[2], 0 };
            v0.N = p;
            v1.N = p;
            v2.N = p;


            // error handlign for correct number of properties per element
        }
        else
        {
            std::cerr << "Incorrect number of faces" << std::endl; 
        }
    }

    perffaces = perf.pop();
    perfoverall = perf.pop();

    fclose(pFile);

    *ppVB = pVerts;
    *ppIB = pIB;

    printf("HEADER: %lf s\n", perfheader);
    printf("VERTS: %lf s\n", perfverts);
    printf("FACES: %lf s\n", perffaces);
    printf("OVERALL: %lf s\n", perfoverall);
}

void Import(
    UINT& numVertices,
    VertexPTN** ppVB,
    UINT& numIndices,
    UINT** ppIB)
{
    ImportCString("../Content/bun_zipper_res4.ply", numVertices, ppVB, numIndices, ppIB);
    //ImportFstream(numVertices, ppVB, numIndices, ppIB);
}

void ImportPly(
    const CHAR* pPlyFilename,
    UINT& numVertices,
    VertexPTN** ppVB,
    UINT& numIndices,
    UINT** ppIB)
{
    ImportCString(pPlyFilename, numVertices, ppVB, numIndices, ppIB);
    //ImportFstream(numVertices, ppVB, numIndices, ppIB);
}
