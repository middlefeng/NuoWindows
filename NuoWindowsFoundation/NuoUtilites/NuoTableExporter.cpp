//
//  NuoTableExporter.cpp
//  ModelViewer
//
//  Created by middleware on 12/6/16.
//  Copyright © 2016 middleware. All rights reserved.
//

#include "NuoTableExporter.h"

#include <memory>


NuoTableExporter::NuoTableExporter()
    : _indent(0)
{
    _result = "return";
}


void NuoTableExporter::StartTable()
{
    _result = _result + "\n";
    AppendByIndent();
    _result = _result + "{\n";
    _indent += 4;
}


void NuoTableExporter::StartEntry(const std::string& entryName)
{
    AppendByIndent();
    _result = _result + entryName + " = ";
}



void NuoTableExporter::EndEntry(bool extraLine)
{
    _result = _result + ",\n";
    if (extraLine)
        _result += "\n";
}



void NuoTableExporter::StartArrayIndex(size_t index)
{
    char* buffer = new char[100];
    snprintf(buffer, 100, "%zu", index);
    
    AppendByIndent();
    _result = _result + "[" + buffer + "] = ";
    
    delete[] buffer;
}



void NuoTableExporter::SetEntryValueFloat(float value)
{
    char* buffer = new char[100];
    snprintf(buffer, 100, "%.6f", value);
    
    _result = _result + buffer;
    
    delete[] buffer;
}

void NuoTableExporter::SetEntryValueBool(bool value)
{
    const char* buffer = value ? "true" : "false";
    _result = _result + buffer;
}



void NuoTableExporter::SetEntryValueString(const std::string& value)
{
     _result = _result + "\"" + value + "\"";
}


void NuoTableExporter::EndTable()
{
    _indent -= 4;
    AppendByIndent();
    _result = _result + "}";
}



void NuoTableExporter::SetMatrix(const NuoMatrixFloat44& matrix)
{
    StartTable();
    
    for (unsigned char col = 0; col < 4; ++ col)
    {
        StartArrayIndex(col);
        StartTable();
        
        NuoVectorFloat4 colomn = matrix[col];
        
        for (unsigned char row = 0; row < 4; ++ row)
        {
            StartArrayIndex(row);
            SetEntryValueFloat(colomn[row]);
            EndEntry(false);
        }
        
        EndTable();
        EndEntry(false);
    }
    
    EndTable();
}



void NuoTableExporter::AppendByIndent()
{
    for (size_t i = 0; i < _indent; ++i)
        _result = _result + " ";
}


const std::string& NuoTableExporter::GetResult() const
{
    return _result;
}


