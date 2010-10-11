/******************************************************************************
 * $Id$
 *
 * Project:  libLAS - http://liblas.org - A BSD library for LAS format data.
 * Purpose:  C API for libLAS
 * Author:   Howard Butler, hobu.inc@gmail.com, but I'm not proud of it.  
 *           I also swiped a lot of it from GDAL.
 *
 ******************************************************************************
 * Copyright (c) 2008, Mateusz Loskot
 * Copyright (c) 2008, Howard Butler
 *
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without 
 * modification, are permitted provided that the following 
 * conditions are met:
 * 
 *     * Redistributions of source code must retain the above copyright 
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright 
 *       notice, this list of conditions and the following disclaimer in 
 *       the documentation and/or other materials provided 
 *       with the distribution.
 *     * Neither the name of the Martin Isenburg or Iowa Department 
 *       of Natural Resources nor the names of its contributors may be 
 *       used to endorse or promote products derived from this software 
 *       without specific prior written permission.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS 
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE 
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, 
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, 
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS 
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED 
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, 
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT 
 * OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
 * OF SUCH DAMAGE.
 ****************************************************************************/

#include <liblas/liblas.hpp>
#include <liblas/lasreader.hpp>
#include <liblas/laserror.hpp>
#include <liblas/laswriter.hpp>
#include <liblas/lasfile.hpp>
#include <liblas/exception.hpp>
#include <liblas/lasvariablerecord.hpp>
#include <liblas/guid.hpp>
#include <liblas/lasspatialreference.hpp>
#include <liblas/capi/las_config.h>
#include <liblas/capi/las_version.h>

typedef struct LASWriterHS *LASWriterH;
typedef struct LASReaderHS *LASReaderH;
typedef struct LASPointHS *LASPointH;
typedef struct LASHeaderHS *LASHeaderH;
typedef struct LASGuidHS *LASGuidH;
typedef struct LASVLRHS *LASVLRH;
typedef struct LASColorHS *LASColorH;
typedef struct LASSRSHS *LASSRSH;


#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream> // std::stringstream
#include <string>
#include <stack>
#include <typeinfo>
#include <vector>
#include <cstdio>

using namespace liblas;

LAS_C_START

#ifndef _WIN32
#include <stdint.h>
#endif

#ifdef _WIN32
#define compare_no_case(a,b,n)  _strnicmp( (a), (b), (n) )
#else
#define compare_no_case(a,b,n)  strncasecmp( (a), (b), (n) )
#endif

// Error stuff


typedef enum
{
    LE_None = 0,
    LE_Debug = 1,
    LE_Warning = 2,
    LE_Failure = 3,
    LE_Fatal = 4
} LASErrorEnum;


static std::stack<LASError > errors;

#ifdef _MSC_VER
# pragma warning(disable: 4127) // warning C4127: conditional expression is constant
#endif

#define VALIDATE_LAS_POINTER0(ptr, func) \
   do { if( NULL == ptr ) { \
        LASErrorEnum const ret = LE_Failure; \
        std::ostringstream msg; \
        msg << "Pointer \'" << #ptr << "\' is NULL in \'" << (func) <<"\'."; \
        std::string message(msg.str()); \
        LASError_PushError( ret, message.c_str(), (func)); \
        return; \
   }} while(0)

#define VALIDATE_LAS_POINTER1(ptr, func, rc) \
   do { if( NULL == ptr ) { \
        LASErrorEnum const ret = LE_Failure; \
        std::ostringstream msg; \
        msg << "Pointer \'" << #ptr << "\' is NULL in \'" << (func) <<"\'."; \
        std::string message(msg.str()); \
        LASError_PushError( ret, message.c_str(), (func)); \
        return (rc); \
   }} while(0)

LAS_DLL int LAS_IsGDALEnabled(void) {
    return IsGDALEnabled();
}

LAS_DLL int LAS_IsLibGeoTIFFEnabled(void) {
    return IsLibGeoTIFFEnabled();
}

LAS_DLL void LASError_Reset(void) {
    if (errors.empty()) return;
    for (std::size_t i=0;i<errors.size();i++) errors.pop();
}

LAS_DLL void LASError_Pop(void) {
    if (errors.empty()) return;
    errors.pop();
}

LAS_DLL int LASError_GetLastErrorNum(void){
    if (errors.empty())
        return 0;
    else {
        LASError err = errors.top();
        return err.GetCode();
    }
}

LAS_DLL char* LASError_GetLastErrorMsg(void){
    if (errors.empty()) 
        return NULL;
    else {
        LASError err = errors.top();
        return strdup(err.GetMessage().c_str());
    }
}

LAS_DLL char* LASError_GetLastErrorMethod(void){
    if (errors.empty()) 
        return NULL;
    else {
        LASError err = errors.top();
        return strdup(err.GetMethod().c_str());
    }
}

LAS_DLL void LASError_PushError(int code, const char *message, const char *method) {
    LASError err = LASError(code, std::string(message), std::string(method));
    errors.push(err);
}

LAS_DLL int LASError_GetErrorCount(void) {
    return static_cast<int>(errors.size());
}

LAS_DLL LASReaderH LASReader_Create(const char* filename) 

{
    VALIDATE_LAS_POINTER1(filename, "LASReader_Create", NULL);

    try {
        std::ios::openmode const mode = std::ios::in | std::ios::binary;
        std::istream* istrm;
        if (compare_no_case(filename,"STDIN",5) == 0)
        {
            istrm = &std::cin;
        }
        else 
        {
            istrm = new std::ifstream(filename, mode);
        }
        
        if (!istrm->good())
        {
            delete istrm;
            throw std::runtime_error("Reading stream was not able to be created");
        }
        return (LASReaderH) new LASReader(*istrm);

    
    } catch (std::exception const& e)
     {
         LASError_PushError(LE_Failure, e.what(), "LASReader_Create");
         return NULL;
     }

    
}

LAS_DLL void LASReader_Destroy(LASReaderH hReader)
{
    VALIDATE_LAS_POINTER0(hReader, "LASReader_Destroy");

    try { 
        LASReader* reader = (LASReader*)hReader;
        std::istream* istrm = &(reader->GetStream());

        delete reader;
        hReader = NULL;
    
        if (static_cast<std::ifstream&>(*istrm))
            static_cast<std::ifstream&>(*istrm).close();
        delete istrm;
        istrm = NULL;
  
        }  catch (std::runtime_error const& e/* e */) 
        {
            LASError_PushError(LE_Failure, e.what(), "LASReader_Destroy");
            return;
        }


    hReader = NULL;
}



LAS_DLL const LASPointH LASReader_GetNextPoint(const LASReaderH hReader)
{
    VALIDATE_LAS_POINTER1(hReader, "LASReader_GetNextPoint", NULL);

    try {
        LASReader *reader = ((LASReader*) hReader);
        if (reader->ReadNextPoint()) 
            // return (LASPointH) new LASPoint(reader->GetPoint());
            return (LASPointH) &(reader->GetPoint());
        else 
            return NULL;
    } catch (invalid_point_data const& e /*e */) {
        LASError_PushError(LE_Failure, e.what(), "LASReader_GetNextPoint Invalid Point");
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASReader_GetNextPoint");
    }
 
    return NULL;
}

LAS_DLL const LASPointH LASReader_GetPointAt(const LASReaderH hReader, liblas::uint32_t position)
{
    VALIDATE_LAS_POINTER1(hReader, "LASReader_GetPointAt", NULL);

    try {
        LASReader *reader = ((LASReader*) hReader);
        if (reader->ReadPointAt((std::size_t) position)) 
            // return (LASPointH) new LASPoint(reader->GetPoint());
            return (LASPointH) &(reader->GetPoint());
        else 
            return NULL;
    } catch (invalid_point_data const& e /*e */) {
        LASError_PushError(LE_Failure, e.what(), "LASReader_GetPointAt Invalid Point");
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASReader_GetPointAt");
    }
 
    return NULL;

}
LAS_DLL LASHeaderH LASReader_GetHeader(const LASReaderH hReader)
{
    VALIDATE_LAS_POINTER1(hReader, "LASReader_GetHeader", NULL);

    LASHeader header = ((LASReader*) hReader)->GetHeader();
    return (LASHeaderH) new LASHeader( header );
}

LAS_DLL LASErrorEnum LASReader_SetSRS(LASHeaderH hReader, const LASSRSH hSRS) {
    
    VALIDATE_LAS_POINTER1(hReader, "LASReader_SetSRS", LE_Failure);
    VALIDATE_LAS_POINTER1(hSRS, "LASReader_SetSRS", LE_Failure);

    try {
        ((LASReader*) hReader)->SetSRS(*((LASSpatialReference*)hSRS));
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASReader_SetSRS");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL LASHeaderH LASHeader_Create(void) {
        return (LASHeaderH) new LASHeader();
}

LAS_DLL LASPointH LASPoint_Create(void) {
        return (LASPointH) new LASPoint();
}

LAS_DLL LASPointH LASPoint_Copy(const LASPointH hPoint) {
        return (LASPointH) new LASPoint(*((LASPoint*) hPoint));
}

LAS_DLL void LASPoint_Destroy(LASPointH hPoint) {
    VALIDATE_LAS_POINTER0(hPoint, "LASPoint_Destroy");
    delete (LASPoint*) hPoint;
    hPoint = NULL;
}

LAS_DLL double LASPoint_GetX(const LASPointH hPoint) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_GetX", 0.0);
    
    double value = ((LASPoint*) hPoint)->GetX();
    return value;
}

LAS_DLL LASErrorEnum LASPoint_SetX(LASPointH hPoint, double value) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_SetX", LE_Failure);

    try {
            ((LASPoint*) hPoint)->SetX(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_SetX");
        return LE_Failure;
    }

    return LE_None;

}

LAS_DLL double LASPoint_GetY(const LASPointH hPoint) {
    
    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_GetY", 0.0);
    
    double value = ((LASPoint*) hPoint)->GetY();
    return value;
}

LAS_DLL LASErrorEnum LASPoint_SetY(LASPointH hPoint, double value) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_SetY", LE_Failure);

    try {
            ((LASPoint*) hPoint)->SetY(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_SetY");
        return LE_Failure;
    }

    return LE_None;

}

LAS_DLL double LASPoint_GetZ(const LASPointH hPoint) {
    
    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_GetZ", 0.0);
    
    double value = ((LASPoint*) hPoint)->GetZ();
    return value;
}

LAS_DLL LASErrorEnum LASPoint_SetZ(LASPointH hPoint, double value) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_SetZ", LE_Failure);

    try {
            ((LASPoint*) hPoint)->SetZ(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_SetZ");
        return LE_Failure;
    }

    return LE_None;

}

LAS_DLL liblas::uint16_t LASPoint_GetIntensity(const LASPointH hPoint) {
    
    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_GetIntensity", 0);
    
    liblas::uint16_t value = ((LASPoint*) hPoint)->GetIntensity();
    return value;
}

LAS_DLL LASErrorEnum LASPoint_SetIntensity(LASPointH hPoint, liblas::uint16_t value) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_SetIntensity", LE_Failure);

    try {
            ((LASPoint*) hPoint)->SetIntensity(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_SetIntensity");
        return LE_Failure;
    }

    return LE_None;

}

LAS_DLL liblas::uint16_t LASPoint_GetReturnNumber(const LASPointH hPoint) {
    
    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_GetReturnNumber", 0);
    
    liblas::uint16_t value = ((LASPoint*) hPoint)->GetReturnNumber();
    return value;
}

LAS_DLL LASErrorEnum LASPoint_SetReturnNumber(LASPointH hPoint, liblas::uint16_t value) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_SetReturnNumber", LE_Failure);

    try {
            ((LASPoint*) hPoint)->SetReturnNumber(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_SetReturnNumber");
        return LE_Failure;
    }

    return LE_None;

}

LAS_DLL liblas::uint16_t LASPoint_GetNumberOfReturns(const LASPointH hPoint) {
    
    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_GetNumberOfReturns", 0);
    
    liblas::uint16_t value = ((LASPoint*) hPoint)->GetNumberOfReturns();
    return value;
}

LAS_DLL LASErrorEnum LASPoint_SetNumberOfReturns(LASPointH hPoint, liblas::uint16_t value) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_SetNumberOfReturns", LE_Failure);

    try {
            ((LASPoint*) hPoint)->SetNumberOfReturns(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_SetNumberOfReturns");
        return LE_Failure;
    }

    return LE_None;

}

LAS_DLL liblas::uint16_t LASPoint_GetScanDirection(const LASPointH hPoint) {
    
    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_GetScanDirection", 0);
    
    liblas::uint16_t value = ((LASPoint*) hPoint)->GetScanDirection();
    return value;
}

LAS_DLL LASErrorEnum LASPoint_SetScanDirection(LASPointH hPoint, liblas::uint16_t value) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_SetScanDirection", LE_Failure);

    try {
            ((LASPoint*) hPoint)->SetScanDirection(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_SetScanDirection");
        return LE_Failure;
    }

    return LE_None;

}

LAS_DLL liblas::uint16_t LASPoint_GetFlightLineEdge(const LASPointH hPoint) {
    
    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_GetFlightLineEdge", 0);
    
    liblas::uint16_t value = ((LASPoint*) hPoint)->GetFlightLineEdge();
    return value;
}

LAS_DLL LASErrorEnum LASPoint_SetFlightLineEdge(LASPointH hPoint, liblas::uint16_t value) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_SetFlightLineEdge", LE_Failure);

    try {
            ((LASPoint*) hPoint)->SetFlightLineEdge(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_SetFlightLineEdge");
        return LE_Failure;
    }

    return LE_None;

}

LAS_DLL liblas::uint8_t LASPoint_GetScanFlags(const LASPointH hPoint) {
    
    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_GetScanFlags", 0);
    
    liblas::uint8_t value = ((LASPoint*) hPoint)->GetScanFlags();
    return value;
}

LAS_DLL LASErrorEnum LASPoint_SetScanFlags(LASPointH hPoint, liblas::uint8_t value) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_SetScanFlags", LE_Failure);

    try {
            ((LASPoint*) hPoint)->SetScanFlags(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_SetScanFlags");
        return LE_Failure;
    }

    return LE_None;

}

LAS_DLL liblas::uint8_t LASPoint_GetClassification(const LASPointH hPoint) {
    
    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_GetClassification", 0);
    
    liblas::uint8_t value = ((LASPoint*) hPoint)->GetClassification();
    return value;
}

LAS_DLL LASErrorEnum LASPoint_SetClassification(LASPointH hPoint, liblas::uint8_t value) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_SetClassification", LE_Failure);

    try {
            ((LASPoint*) hPoint)->SetClassification(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_SetClassification");
        return LE_Failure;
    }

    return LE_None;

}

LAS_DLL LASErrorEnum LASPoint_SetTime(LASPointH hPoint, double value) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_SetTime", LE_Failure);

    try {
            ((LASPoint*) hPoint)->SetTime(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_SetTime");
        return LE_Failure;
    }

    return LE_None;

}

LAS_DLL double LASPoint_GetTime(const LASPointH hPoint) {
    
    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_GetTime", 0.0);
    
    double value = ((LASPoint*) hPoint)->GetTime();
    return value;
}

LAS_DLL liblas::int8_t LASPoint_GetScanAngleRank(const LASPointH hPoint) {
    
    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_GetScanAngleRank", 0);
    
    liblas::int8_t value = ((LASPoint*) hPoint)->GetScanAngleRank();
    return value;
}

LAS_DLL LASErrorEnum LASPoint_SetScanAngleRank(LASPointH hPoint, liblas::int8_t value) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_SetScanAngleRank", LE_Failure);

    try {
            ((LASPoint*) hPoint)->SetScanAngleRank(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_SetScanAngleRank");
        return LE_Failure;
    }

    return LE_None;

}

LAS_DLL liblas::uint16_t LASPoint_GetPointSourceId(const LASPointH hPoint) {
    
    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_GetPointSourceId", 0);
    
    liblas::uint16_t value = ((LASPoint*) hPoint)->GetPointSourceID();
    return value;
}

LAS_DLL LASErrorEnum LASPoint_SetPointSourceId(LASPointH hPoint, liblas::uint16_t value) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_SetPointSourceId", LE_Failure);

    try {
            ((LASPoint*) hPoint)->SetPointSourceID(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_SetPointSourceId");
        return LE_Failure;
    }

    return LE_None;

}


LAS_DLL liblas::uint8_t LASPoint_GetUserData(const LASPointH hPoint) {
    
    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_GetUserData", 0);
    
    liblas::uint8_t value = ((LASPoint*) hPoint)->GetUserData();
    return value;
}

LAS_DLL LASErrorEnum LASPoint_SetUserData(LASPointH hPoint, liblas::uint8_t value) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_SetUserData", LE_Failure);

    try {
            ((LASPoint*) hPoint)->SetUserData(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_SetUserData");
        return LE_Failure;
    }

    return LE_None;

}

LAS_DLL int LASPoint_Equal(const LASPointH hPoint1, const LASPointH hPoint2) {
    VALIDATE_LAS_POINTER1(hPoint1, "LASPoint_Equal", 0);
    VALIDATE_LAS_POINTER1(hPoint2, "LASPoint_Equal", 0);

    LASPoint* point1 = ((LASPoint*) hPoint1);
    LASPoint* point2 = ((LASPoint*) hPoint2);

    return (point1 == point2);

}

LAS_DLL int LASPoint_Validate(LASPointH hPoint) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_Validate", LE_Failure);

    try {
            ((LASPoint*) hPoint)->Validate();
    } catch (invalid_point_data const& e /*e */) {
        return e.who();
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_Validate");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL int LASPoint_IsValid(LASPointH hPoint) {

    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_IsValid", LE_Failure);
    return ((LASPoint*) hPoint)->IsValid();
}

LAS_DLL char* LASHeader_GetFileSignature(const LASHeaderH hHeader) {
    // caller owns it
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetFileSignature", NULL);
    
    std::string signature = ((LASHeader*) hHeader)->GetFileSignature();
    return strdup(signature.c_str());
}

LAS_DLL liblas::uint16_t LASHeader_GetFileSourceId(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetFileSourceId", 0);

    unsigned short value = ((LASHeader*) hHeader)->GetFileSourceId();
    return value;
}

LAS_DLL LASErrorEnum LASHeader_SetFileSourceId(LASHeaderH hHeader, liblas::uint16_t value) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetFileSourceId", LE_Failure);
    ((LASHeader*) hHeader)->SetFileSourceId(value);    
    return LE_None;
}


LAS_DLL liblas::uint16_t LASHeader_GetReserved(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetReserved", 0);

    unsigned short value = ((LASHeader*) hHeader)->GetReserved();
    return value;
}

LAS_DLL LASErrorEnum LASHeader_SetReserved(LASHeaderH hHeader, liblas::uint16_t value) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetReserved", LE_Failure);
    ((LASHeader*) hHeader)->SetReserved(value);    
    return LE_None;
}

LAS_DLL char* LASHeader_GetProjectId(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetProjectId", 0);
    
    liblas::guid id = ((LASHeader*) hHeader)->GetProjectId();
    return strdup(id.to_string().c_str());
}

LAS_DLL LASErrorEnum LASHeader_SetProjectId(LASHeaderH hHeader, const char* value) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetProjectId", LE_Failure);

    try {
        liblas::guid id;
        id = liblas::guid::guid(value);
        ((LASHeader*) hHeader)->SetProjectId(id);    
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_SetGUID");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL liblas::uint8_t LASHeader_GetVersionMajor(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetVersionMajor", 0);

    long value = ((LASHeader*) hHeader)->GetVersionMajor();
    return liblas::uint8_t(value);
}

LAS_DLL LASErrorEnum LASHeader_SetVersionMajor(LASHeaderH hHeader, liblas::uint8_t value) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetVersionMajor", LE_Failure);

    try {
        ((LASHeader*) hHeader)->SetVersionMajor(value);    
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_SetVersionMajor");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL liblas::uint8_t LASHeader_GetVersionMinor(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetVersionMinor", 0);

    long value = ((LASHeader*) hHeader)->GetVersionMinor();
    return liblas::uint8_t(value);
}

LAS_DLL LASErrorEnum LASHeader_SetVersionMinor(LASHeaderH hHeader, liblas::uint8_t value) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetVersionMinor", LE_Failure);

    // TODO: Maybe this should be a fatal error -- hobu
    try {
        ((LASHeader*) hHeader)->SetVersionMinor(value);    
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_SetVersionMinor");
        return LE_Failure;
    }
 
    return LE_None;
}

LAS_DLL char* LASHeader_GetSystemId(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetSystemId", NULL);

    // caller owns it
    std::string sysid = ((LASHeader*) hHeader)->GetSystemId();
    return strdup(sysid.c_str());
}

LAS_DLL LASErrorEnum LASHeader_SetSystemId(LASHeaderH hHeader, const char* value) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetSystemId", LE_Failure); 

    try {
            ((LASHeader*) hHeader)->SetSystemId(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_SetSystemId");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL char* LASHeader_GetSoftwareId(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetSoftwareId", NULL);

    // caller owns it
    std::string softid = ((LASHeader*) hHeader)->GetSoftwareId();
    return strdup(softid.c_str());
}

LAS_DLL LASErrorEnum LASHeader_SetSoftwareId(LASHeaderH hHeader, const char* value) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetSoftwareId", LE_Failure); 

    try {
            ((LASHeader*) hHeader)->SetSoftwareId(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_SetSoftwareId");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL liblas::uint16_t LASHeader_GetCreationDOY(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetCreationDOY", 0);

    unsigned short value = ((LASHeader*) hHeader)->GetCreationDOY();
    return value;
}

LAS_DLL LASErrorEnum LASHeader_SetCreationDOY(LASHeaderH hHeader, liblas::uint16_t value) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetCreationDOY", LE_Failure);
    ((LASHeader*) hHeader)->SetCreationDOY(value);    
    return LE_None;
}

LAS_DLL liblas::uint16_t LASHeader_GetCreationYear(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetCreationYear", 0);

    unsigned short value = ((LASHeader*) hHeader)->GetCreationYear();
    return value;
}

LAS_DLL LASErrorEnum LASHeader_SetCreationYear(LASHeaderH hHeader, liblas::uint16_t value) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetCreationYear", LE_Failure);
    ((LASHeader*) hHeader)->SetCreationYear(value);    
    return LE_None;
}

LAS_DLL liblas::uint16_t LASHeader_GetHeaderSize(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetHeaderSize", 0);

    unsigned short value = ((LASHeader*) hHeader)->GetHeaderSize();
    return value;
}

LAS_DLL liblas::uint32_t LASHeader_GetDataOffset(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetDataOffset", 0);

    unsigned long value = ((LASHeader*) hHeader)->GetDataOffset();
    return value;
}

LAS_DLL LASErrorEnum LASHeader_SetDataOffset(const LASHeaderH hHeader, liblas::uint32_t value) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetDataOffset", LE_Failure);

    try {
        ((LASHeader*) hHeader)->SetDataOffset(value);    
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_SetDataOffset");
        return LE_Failure;
    }
 
    return LE_None;    
}


LAS_DLL liblas::uint32_t LASHeader_GetRecordsCount(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetRecordsCount", 0);

    unsigned long value = ((LASHeader*) hHeader)->GetRecordsCount();
    return value;
}

LAS_DLL liblas::uint8_t LASHeader_GetDataFormatId(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetDataFormatId", 0);
    
    LASHeader::PointFormat id = ((LASHeader*) hHeader)->GetDataFormatId();
    return static_cast<liblas::uint8_t>(id);
}

LAS_DLL LASErrorEnum LASHeader_SetDataFormatId(LASHeaderH hHeader, liblas::uint8_t value) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetDataFormatId", LE_Failure); 
    
    try {
            ((LASHeader*) hHeader)->SetDataFormatId((liblas::LASHeader::PointFormat)value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_SetDataFormatId");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL liblas::uint16_t LASHeader_GetDataRecordLength(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetDataRecordLength", 0);

    unsigned short value = ((LASHeader*) hHeader)->GetDataRecordLength();
    return value;
}


LAS_DLL liblas::uint32_t LASHeader_GetPointRecordsByReturnCount(const LASHeaderH hHeader, int index) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetPointRecordsByReturnCount", 0);

    std::vector<liblas::uint32_t> counts  = ((LASHeader*) hHeader)->GetPointRecordsByReturnCount();
    if ( (index < 5) && (index >= 0)) {
        return counts[index];
    } 

    return 0;
    
}

LAS_DLL LASErrorEnum LASHeader_SetPointRecordsByReturnCount(const LASHeaderH hHeader, int index, liblas::uint32_t value) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetPointRecordsByReturnCount", LE_Failure);

    try {
        ((LASHeader*) hHeader)->SetPointRecordsByReturnCount(index, value);    
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_SetPointRecordsByReturnCount");
        return LE_Failure;
    }
 
    return LE_None;    
}


LAS_DLL liblas::uint32_t LASHeader_GetPointRecordsCount(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetPointRecordsCount", 0);

    unsigned long value = ((LASHeader*) hHeader)->GetPointRecordsCount();
    return value;
}

LAS_DLL LASErrorEnum LASHeader_SetPointRecordsCount(const LASHeaderH hHeader, liblas::uint32_t value) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetPointRecordsCount", LE_Failure);

    try {
        ((LASHeader*) hHeader)->SetPointRecordsCount(value);    
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_SetPointRecordsCount");
        return LE_Failure;
    }
 
    return LE_None;    
}

LAS_DLL double LASHeader_GetScaleX(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetScaleX", 0.0);

    double value = ((LASHeader*) hHeader)->GetScaleX();
    return value;
}

LAS_DLL double LASHeader_GetScaleY(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetScaleY", 0.0);

    double value = ((LASHeader*) hHeader)->GetScaleY();
    return value;
}

LAS_DLL double LASHeader_GetScaleZ(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetScaleZ", 0.0);

    double value = ((LASHeader*) hHeader)->GetScaleZ();
    return value;
}

LAS_DLL LASErrorEnum LASHeader_SetScale(LASHeaderH hHeader, double x, double y, double z) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetScale", LE_Failure); 
    
    try {
            ((LASHeader*) hHeader)->SetScale(x,y,z);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_SetScale");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL double LASHeader_GetOffsetX(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetOffsetX", 0.0);

    double value = ((LASHeader*) hHeader)->GetOffsetX();
    return value;
}

LAS_DLL double LASHeader_GetOffsetY(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetOffsetY", 0.0);

    double value = ((LASHeader*) hHeader)->GetOffsetY();
    return value;
}

LAS_DLL double LASHeader_GetOffsetZ(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetOffsetZ", 0.0);

    double value = ((LASHeader*) hHeader)->GetOffsetZ();
    return value;
}

LAS_DLL LASErrorEnum LASHeader_SetOffset(LASHeaderH hHeader, double x, double y, double z) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetOffset", LE_Failure); 
    
    try {
            ((LASHeader*) hHeader)->SetOffset(x,y,z);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_SetOffset");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL double LASHeader_GetMinX(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetMinX", 0.0);

    double value = ((LASHeader*) hHeader)->GetMinX();
    return value;
}

LAS_DLL double LASHeader_GetMinY(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetMinY", 0.0);

    double value = ((LASHeader*) hHeader)->GetMinY();
    return value;
}

LAS_DLL double LASHeader_GetMinZ(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetMinZ", 0.0);

    double value = ((LASHeader*) hHeader)->GetMinZ();
    return value;
}

LAS_DLL LASErrorEnum LASHeader_SetMin(LASHeaderH hHeader, double x, double y, double z) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetMin", LE_Failure); 
    
    try {
            ((LASHeader*) hHeader)->SetMin(x,y,z);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_SetMin");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL double LASHeader_GetMaxX(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetMaxX", 0.0);

    double value = ((LASHeader*) hHeader)->GetMaxX();
    return value;
}

LAS_DLL double LASHeader_GetMaxY(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetMaxY", 0.0);

    double value = ((LASHeader*) hHeader)->GetMaxY();
    return value;
}

LAS_DLL double LASHeader_GetMaxZ(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetMaxZ", 0.0);

    double value = ((LASHeader*) hHeader)->GetMaxZ();
    return value;
}

LAS_DLL LASErrorEnum LASHeader_SetMax(LASHeaderH hHeader, double x, double y, double z) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetMax", LE_Failure); 
    
    try {
            ((LASHeader*) hHeader)->SetMax(x,y,z);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_SetMax");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL void LASHeader_Destroy(LASHeaderH hHeader)
{
    VALIDATE_LAS_POINTER0(hHeader, "LASHeader_Destroy");
    delete ((LASHeader*) hHeader);
    hHeader=NULL;
}

LAS_DLL LASHeaderH LASHeader_Copy(const LASHeaderH hHeader) {
        return (LASHeaderH) new LASHeader(*((LASHeader*) hHeader));
}

LAS_DLL int LASHeader_Equal(const LASHeaderH hHeader1, const LASHeaderH hHeader2) {
    VALIDATE_LAS_POINTER1(hHeader1, "LASHeader_Equal", 0);
    VALIDATE_LAS_POINTER1(hHeader2, "LASHeader_Equal", 0);

    LASHeader* header1 = ((LASHeader*) hHeader1);
    LASHeader* header2 = ((LASHeader*) hHeader2);

    return (header1 == header2);
}

LAS_DLL LASGuidH LASHeader_GetGUID(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetGUID", 0);
    
    liblas::guid id = ((LASHeader*) hHeader)->GetProjectId();
    return (LASGuidH) new liblas::guid(id);
}


LAS_DLL LASErrorEnum LASHeader_SetGUID(LASHeaderH hHeader, LASGuidH hId) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetGUID", LE_Failure);

    try {
        liblas::guid* id = (liblas::guid*) hId;
        
        ((LASHeader*) hHeader)->SetProjectId(*id);    
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_SetGUID");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL LASVLRH LASHeader_GetVLR(const LASHeaderH hHeader, liblas::uint32_t i) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetVLR", 0);
    
    LASVariableRecord vlr = ((LASHeader*) hHeader)->GetVLR(i);
    return (LASVLRH) new LASVariableRecord(vlr);
}

LAS_DLL LASErrorEnum LASHeader_DeleteVLR(LASHeaderH hHeader, liblas::uint32_t index) {
    
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_DeleteVLR", LE_Failure);

    try {
        ((LASHeader*) hHeader)->DeleteVLR(index);
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_DeleteVLR");
        return LE_Failure;
    }


    return LE_None;
}

LAS_DLL LASErrorEnum LASHeader_AddVLR(LASHeaderH hHeader, const LASVLRH hVLR) {
    
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_AddVLR", LE_Failure);
    VALIDATE_LAS_POINTER1(hVLR, "LASHeader_AddVLR", LE_Failure);

    try {
        ((LASHeader*) hHeader)->AddVLR(*((LASVariableRecord*)hVLR));
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_AddVLR");
        return LE_Failure;
    }


    return LE_None;
}




LAS_DLL LASWriterH LASWriter_Create(const char* filename, const LASHeaderH hHeader, int mode) {
    VALIDATE_LAS_POINTER1(hHeader, "LASWriter_Create", NULL); 
    
    if (filename == NULL) {
        LASError_PushError(LE_Failure, "Input filename was null", "LASWriter_Create");
        return NULL;
    }
    try {
        std::ios::openmode m;
        if ( (mode > 2) || (mode < 1)) {
            throw std::runtime_error("File mode must be eWrite or eAppend");
        }
        
        std::ostream* ostrm;

        // append mode 
        if (mode == 2) {
            m = std::ios::out | std::ios::in | std::ios::binary | std::ios::ate;
        }
        // write mode
        else {
            m = std::ios::out | std::ios::binary | std::ios::ate;
        }
                
        if (compare_no_case(filename,"STOUT",5) == 0)
        {
            ostrm = &std::cout;
        }
        else 
        {
            ostrm = new std::ofstream(filename, m);
        }

        
        if (!ostrm->good())
        {
            delete ostrm;
            throw std::runtime_error("Writing stream was not able to be created");
        }
        
        LASHeader* header = ((LASHeader*) hHeader);
        LASWriter* writer = new LASWriter(*ostrm, *header);
        return (LASWriterH) writer;

    } catch (std::exception const& e)
     {
         LASError_PushError(LE_Failure, e.what(), "LASWriter_Create");
         return NULL;
     }
    
}

LAS_DLL LASErrorEnum LASWriter_WritePoint(const LASWriterH hWriter, const LASPointH hPoint) {

    VALIDATE_LAS_POINTER1(hPoint, "LASWriter_WritePoint", LE_Failure);
    int ret;

    try {
            ret = ((LASWriter*) hWriter)->WritePoint(*((LASPoint*) hPoint));
            if (!ret) {
                LASError_PushError( LE_Warning, 
                                    "Failed to write point because it was invalid", 
                                    "LASWriter_WritePoint");
                return LE_Warning;                
            }
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASWriter_WritePoint");
        return LE_Failure;
    }

    return LE_None;    
}

LAS_DLL LASErrorEnum LASWriter_WriteHeader(const LASWriterH hWriter, const LASHeaderH hHeader) {

    VALIDATE_LAS_POINTER1(hHeader, "LASWriter_WriteHeader", LE_Failure);
    VALIDATE_LAS_POINTER1(hWriter, "LASWriter_WriteHeader", LE_Failure);
    
    try {
            ((LASWriter*) hWriter)->WriteHeader(*((LASHeader*) hHeader));
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASWriter_WriteHeader");
        return LE_Failure;
    }

    return LE_None;    
}

LAS_DLL void LASWriter_Destroy(LASWriterH hWriter)
{
    VALIDATE_LAS_POINTER0(hWriter, "LASWriter_Destroy");


    

  
    
    try { 
        LASWriter* writer = (LASWriter*)hWriter;
        std::ostream* ostrm = &(writer->GetStream());

        delete writer;
        hWriter = NULL;
    
        if (static_cast<std::ofstream&>(*ostrm))
            static_cast<std::ofstream&>(*ostrm).close();
        delete ostrm;
        ostrm = NULL;
  
        }  catch (std::runtime_error const& e/* e */) 
        {
            LASError_PushError(LE_Failure, e.what(), "LASWriter_Destroy");
            return;
        }

}

LAS_DLL LASErrorEnum LASWriter_SetSRS(LASWriterH hWriter, const LASSRSH hSRS) {
    
    VALIDATE_LAS_POINTER1(hWriter, "LASWriter_SetSRS", LE_Failure);
    VALIDATE_LAS_POINTER1(hSRS, "LASWriter_SetSRS", LE_Failure);

    try {
        ((LASWriter*) hWriter)->SetSRS(*((LASSpatialReference*)hSRS));
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASWriter_SetSRS");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL void LASError_Print(const char* message) {

    char* errmsg= NULL;
    char* errmethod = NULL;
    errmsg = LASError_GetLastErrorMsg();
    errmethod = LASError_GetLastErrorMethod();
    if (LASError_GetErrorCount()) {
        fprintf(stderr, 
            "%s: %s (%d) from method %s\n",
            message,
            errmsg,
            LASError_GetLastErrorNum(),
            errmethod
        ); 
        if (errmsg) free(errmsg);
        if (errmethod) free(errmethod);
    } else {
        fprintf(stderr, 
            "You have encountered an error. '%s'\n",
            message
        );         
    }

}

LAS_DLL char * LAS_GetVersion() {

    /* XXX - mloskot: I'd suggest to define PACKAGE_VERSION as static object
       and return safe const pointer, instead of newly allocated string. */

    /* FIXME - mloskot: Termporarily, I defined PACKAGE_VERSION
       in las_config.h to solve Visual C++ compilation  (Ticket #23) */

    std::ostringstream output;
        
//    output << LIBLAS_VERSION_MAJOR;
//    output << "." << LIBLAS_VERSION_MINOR;
//    output << "." << LIBLAS_VERSION_REV;
//    output << "." << LIBLAS_VERSION_BUILD;
    output << LIBLAS_RELEASE_NAME;
    std::string out(output.str());
    return strdup(out.c_str());
}



LAS_DLL LASVLRH LASVLR_Create(void) {
    return (LASVLRH) new LASVariableRecord();
}

LAS_DLL void LASVLR_Destroy(LASVLRH hVLR){
    VALIDATE_LAS_POINTER0(hVLR, "LASVLR_Destroy");
    delete (LASVariableRecord*)hVLR;
    hVLR = NULL;
    
}

LAS_DLL char* LASVLR_GetUserId(const LASVLRH hVLR) {
    VALIDATE_LAS_POINTER1(hVLR, "LASVLR_GetUserId", 0);
    LASVariableRecord* vlr = (LASVariableRecord*)hVLR;
    return strdup(vlr->GetUserId(true).c_str());
}

LAS_DLL LASErrorEnum LASVLR_SetUserId(LASVLRH hVLR, const char* value) {
    VALIDATE_LAS_POINTER1(hVLR, "LASVLR_SetUserId", LE_Failure); 

    try {
            ((LASVariableRecord*) hVLR)->SetUserId(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASVLR_SetUserId");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL char* LASVLR_GetDescription(const LASVLRH hVLR) {
    VALIDATE_LAS_POINTER1(hVLR, "LASVLR_GetDescription", 0);
    LASVariableRecord* vlr = (LASVariableRecord*)hVLR;
    return strdup(vlr->GetDescription(true).c_str());
}

LAS_DLL LASErrorEnum LASVLR_SetDescription(LASVLRH hVLR, const char* value) {
    VALIDATE_LAS_POINTER1(hVLR, "LASVLR_SetDescription", LE_Failure); 

    try {
            ((LASVariableRecord*) hVLR)->SetDescription(value);
    } catch (std::exception const& e)
    {
        LASError_PushError(LE_Failure, e.what(), "LASVLR_SetDescription");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL liblas::uint16_t LASVLR_GetRecordLength(const LASVLRH hVLR) {
    
    VALIDATE_LAS_POINTER1(hVLR, "LASVLR_GetRecordLength", 0);
    
    liblas::uint16_t value = ((LASVariableRecord*) hVLR)->GetRecordLength();
    return value;
}
LAS_DLL LASErrorEnum LASVLR_SetRecordLength(LASVLRH hVLR, liblas::uint16_t value) {
    VALIDATE_LAS_POINTER1(hVLR, "LASVLR_SetRecordLength", LE_Failure);
    ((LASVariableRecord*) hVLR)->SetRecordLength(value);    
    return LE_None;
}

LAS_DLL liblas::uint16_t LASVLR_GetRecordId(const LASVLRH hVLR) {
    
    VALIDATE_LAS_POINTER1(hVLR, "LASVLR_GetRecordId", 0);
    
    liblas::uint16_t value = ((LASVariableRecord*) hVLR)->GetRecordId();
    return value;
}
LAS_DLL LASErrorEnum LASVLR_SetRecordId(LASVLRH hVLR, liblas::uint16_t value) {
    VALIDATE_LAS_POINTER1(hVLR, "LASVLR_SetRecordId", LE_Failure);
    ((LASVariableRecord*) hVLR)->SetRecordId(value);    
    return LE_None;
}


LAS_DLL LASErrorEnum LASVLR_SetReserved(LASVLRH hVLR, liblas::uint16_t value) {
    VALIDATE_LAS_POINTER1(hVLR, "LASVLR_SetReserved", LE_Failure);
    ((LASVariableRecord*) hVLR)->SetReserved(value);    
    return LE_None;
}

LAS_DLL liblas::uint16_t LASVLR_GetReserved(const LASVLRH hVLR) {
    
    VALIDATE_LAS_POINTER1(hVLR, "LASVLR_GetReserved", 0);
    
    liblas::uint16_t value = ((LASVariableRecord*) hVLR)->GetReserved();
    return value;
}

LAS_DLL LASErrorEnum LASVLR_GetData(const LASVLRH hVLR, liblas::uint8_t* data) {
    
    VALIDATE_LAS_POINTER1(hVLR, "LASVLR_GetData", LE_Failure);

    try {
        LASVariableRecord* vlr = ((LASVariableRecord*) hVLR);
        std::vector<liblas::uint8_t> d = vlr->GetData();
        liblas::uint16_t length = vlr->GetRecordLength();
        for (liblas::uint16_t i=0; i < length; i++) {
            data[i] = d[i];
        }
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASVLR_GetData");
        return LE_Failure;
    }


    return LE_None;
}

LAS_DLL LASErrorEnum LASVLR_SetData(const LASVLRH hVLR, liblas::uint8_t* data, liblas::uint16_t length) {
    
    VALIDATE_LAS_POINTER1(hVLR, "LASVLR_SetData", LE_Failure);

    try {
        LASVariableRecord* vlr = ((LASVariableRecord*) hVLR);
        std::vector<liblas::uint8_t> d;
        d.resize(length);
        for (liblas::uint16_t i=0; i < length; i++) {
            d[i] = data[i];
        }
        vlr->SetData(d);
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASVLR_GetData");
        return LE_Failure;
    }


    return LE_None;
}

LAS_DLL LASGuidH LASGuid_Create() {
    liblas::guid random;
    try {
        random = liblas::guid::create();
        return (LASGuidH) new liblas::guid(random);
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASGuid_Create");
        return NULL;
    }
}

LAS_DLL LASGuidH LASGuid_CreateFromString(const char* string) {
    VALIDATE_LAS_POINTER1(string, "LASGuid_CreateFromString", NULL);    
    liblas::guid id;
    try {
        id = liblas::guid::guid(string);
        return (LASGuidH) new liblas::guid(id);
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASGuid_CreateFromString");
        return NULL;
    }
}

LAS_DLL void LASGuid_Destroy(LASGuidH hId) {
    VALIDATE_LAS_POINTER0(hId, "LASGuid_Destroy");
    delete (liblas::guid*) hId;
    hId = NULL;
}

LAS_DLL int LASGuid_Equals(LASGuidH hId1, LASGuidH hId2) {
    VALIDATE_LAS_POINTER1(hId1, "LASGuid_Equals", LE_Failure);
    VALIDATE_LAS_POINTER1(hId2, "LASGuid_Equals", LE_Failure);

    liblas::guid* id1 = (liblas::guid*)hId1;
    liblas::guid* id2 = (liblas::guid*)hId2;
    try {

        return( *id1 == *id2);
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASGuid_Equals");
        return LE_Failure;
    }
}

LAS_DLL char* LASGuid_AsString(LASGuidH hId) {
    VALIDATE_LAS_POINTER1(hId, "LASGuid_AsString", 0);
    liblas::guid* id= (liblas::guid*)hId;
    return strdup(id->to_string().c_str());
}



LAS_DLL LASColorH LASColor_Create(void) {
    return (LASColorH) new LASColor();
}

LAS_DLL void LASColor_Destroy(LASColorH hColor){
    VALIDATE_LAS_POINTER0(hColor, "LASColor_Destroy");
    delete (LASColor*)hColor;
    hColor = NULL;
}

LAS_DLL LASErrorEnum LASColor_SetRed(LASColorH hColor, liblas::uint16_t value) {
    
    VALIDATE_LAS_POINTER1(hColor, "LASColor_SetRed", LE_Failure);

    try {
        LASColor* color = ((LASColor*) hColor);
        color->SetRed(value);
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASColor_SetRed");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL liblas::uint16_t LASColor_GetRed(LASColorH hColor) {
    
    VALIDATE_LAS_POINTER1(hColor, "LASColor_GetRed", 0);
    
    liblas::uint16_t value = ((LASColor*) hColor)->GetRed();
    return value;
}

LAS_DLL LASErrorEnum LASColor_SetBlue(LASColorH hColor, liblas::uint16_t value) {
    
    VALIDATE_LAS_POINTER1(hColor, "LASColor_SetBlue", LE_Failure);

    try {
        LASColor* color = ((LASColor*) hColor);
        color->SetBlue(value);
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASColor_SetBlue");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL liblas::uint16_t LASColor_GetBlue(LASColorH hColor) {
    
    VALIDATE_LAS_POINTER1(hColor, "LASColor_GetBlue", 0);
    
    liblas::uint16_t value = ((LASColor*) hColor)->GetBlue();
    return value;
}

LAS_DLL LASErrorEnum LASColor_SetGreen(LASColorH hColor, liblas::uint16_t value) {
    
    VALIDATE_LAS_POINTER1(hColor, "LASColor_SetGreen", LE_Failure);

    try {
        LASColor* color = ((LASColor*) hColor);
        color->SetGreen(value);
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASColor_SetGreen");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL liblas::uint16_t LASColor_GetGreen(LASColorH hColor) {
    
    VALIDATE_LAS_POINTER1(hColor, "LASColor_GetGreen", 0);
    
    liblas::uint16_t value = ((LASColor*) hColor)->GetGreen();
    return value;
}

LAS_DLL LASColorH LASPoint_GetColor(const LASPointH hPoint) {
    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_GetColor", 0);
    
    LASColor color = ((LASPoint*) hPoint)->GetColor();
    return (LASColorH) new LASColor(color);
}

LAS_DLL LASErrorEnum LASPoint_SetColor(LASPointH hPoint, const LASColorH hColor) {
    
    VALIDATE_LAS_POINTER1(hPoint, "LASPoint_SetColor", LE_Failure);
    VALIDATE_LAS_POINTER1(hColor, "LASPoint_SetColor", LE_Failure);

    try {
        ((LASPoint*) hPoint)->SetColor(*((LASColor*)hColor));
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASPoint_SetColor");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL LASSRSH LASSRS_Create(void) {
    return (LASSRSH) new LASSpatialReference();
}

LAS_DLL void LASSRS_Destroy(LASSRSH hSRS){
    VALIDATE_LAS_POINTER0(hSRS, "LASSRS_Destroy");
    delete (LASSpatialReference*)hSRS;
    hSRS = NULL;
}

LAS_DLL const GTIF* LASSRS_GetGTIF(LASSRSH hSRS) {
    VALIDATE_LAS_POINTER1(hSRS, "LASSRS_GetGTIF", 0);
    
    try {
        return ((LASSpatialReference*) hSRS)->GetGTIF();
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASSRS_GetGTIF");
        return 0;
    }
}

LAS_DLL LASErrorEnum LASSRS_SetGTIF(LASSRSH hSRS, const GTIF* gtiff, const ST_TIFF* tiff)
{
    VALIDATE_LAS_POINTER1(hSRS, "LASSRS_SetGTIF", LE_Failure);
    VALIDATE_LAS_POINTER1(gtiff, "LASSRS_SetGTIF", LE_Failure);
    VALIDATE_LAS_POINTER1(tiff, "LASSRS_SetGTIF", LE_Failure);
    try {
        ((LASSpatialReference*) hSRS)->SetGTIF(gtiff, tiff);
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASSRS_SetGTIF");
        return LE_Failure;
    }

    return LE_None;    
}
LAS_DLL char* LASSRS_GetProj4(LASSRSH hSRS) 
{
    VALIDATE_LAS_POINTER1(hSRS, "LASSRS_GetProj4", NULL);
    LASSpatialReference* srs = (LASSpatialReference*)hSRS;

    return strdup((srs)->GetProj4().c_str());
    
}

LAS_DLL LASErrorEnum LASSRS_SetProj4(LASSRSH hSRS, const char* value)
{
    VALIDATE_LAS_POINTER1(hSRS, "LASSRS_SetProj4", LE_Failure);
    VALIDATE_LAS_POINTER1(value, "LASSRS_SetProj4", LE_Failure);

    try {
         ((LASSpatialReference*) hSRS)->SetProj4(value);
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASSRS_SetProj4");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL char* LASSRS_GetWKT(LASSRSH hSRS) 
{
    VALIDATE_LAS_POINTER1(hSRS, "LASSRS_GetWKT", NULL);
    LASSpatialReference* srs = (LASSpatialReference*)hSRS;

    return strdup((srs)->GetWKT().c_str());
    
}

LAS_DLL LASErrorEnum LASSRS_SetWKT(LASSRSH hSRS, const char* value)
{
    VALIDATE_LAS_POINTER1(hSRS, "LASSRS_SetWKT", LE_Failure);
    VALIDATE_LAS_POINTER1(value, "LASSRS_SetWKT", LE_Failure);

    try {
         ((LASSpatialReference*) hSRS)->SetWKT(value);
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASSRS_SetWKT");
        return LE_Failure;
    }

    return LE_None;
}

LAS_DLL LASErrorEnum LASSRS_AddVLR(LASSRSH hSRS, const LASVLRH hVLR) {
    
    VALIDATE_LAS_POINTER1(hSRS, "LASSRS_AddVLR", LE_Failure);
    VALIDATE_LAS_POINTER1(hVLR, "LASSRS_AddVLR", LE_Failure);

    try {
        ((LASSpatialReference*) hSRS)->AddVLR(*((LASVariableRecord*)hVLR));
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASSRS_AddVLR");
        return LE_Failure;
    }


    return LE_None;
}

LAS_DLL LASVLRH LASSRS_GetVLR(const LASSRSH hSRS, liblas::uint32_t i) {
    VALIDATE_LAS_POINTER1(hSRS, "LASSRS_GetVLR", 0);
    
    LASVariableRecord vlr = ((LASSpatialReference*) hSRS)->GetVLRs()[i];
    return (LASVLRH) new LASVariableRecord(vlr);
}

LAS_DLL liblas::uint32_t LASSRS_GetVLRCount(const LASSRSH hSRS) {
    VALIDATE_LAS_POINTER1(hSRS, "LASSRS_GetVLR", 0);
    
    liblas::uint32_t size = ((LASSpatialReference*) hSRS)->GetVLRs().size();
    return size;
}

LAS_DLL LASErrorEnum LASHeader_SetSRS(LASHeaderH hHeader, const LASSRSH hSRS) {
    
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_SetSRS", LE_Failure);
    VALIDATE_LAS_POINTER1(hSRS, "LASHeader_SetSRS", LE_Failure);

    try {
        ((LASHeader*) hHeader)->SetSRS(*((LASSpatialReference*)hSRS));
    }
    catch (std::exception const& e) {
        LASError_PushError(LE_Failure, e.what(), "LASHeader_SetSRS");
        return LE_Failure;
    }


    return LE_None;
}

LAS_DLL LASSRSH LASHeader_GetSRS(const LASHeaderH hHeader) {
    VALIDATE_LAS_POINTER1(hHeader, "LASHeader_GetSRS", 0);
    
    LASSpatialReference srs = ((LASHeader*) hHeader)->GetSRS();
    return (LASSRSH) new LASSpatialReference(srs);
}


LAS_DLL void LASString_Free(char* string) {
    if (string)
        free(string);
}

LAS_C_END

#ifdef _MSC_VER
# pragma warning(default: 4127) // enable warning C4127: conditional expression is constant
#endif
