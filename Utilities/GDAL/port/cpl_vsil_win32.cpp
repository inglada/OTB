/**********************************************************************
 * $Id: cpl_vsil_win32.cpp,v 1.13 2005/09/29 03:24:56 fwarmerdam Exp $
 *
 * Project:  CPL - Common Portability Library
 * Purpose:  Implement VSI large file api for Win32.
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 **********************************************************************
 * Copyright (c) 2000, Frank Warmerdam <warmerdam@pobox.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
 * DEALINGS IN THE SOFTWARE.
 **********************************************************************
 *
 * $Log: cpl_vsil_win32.cpp,v $
 * Revision 1.13  2005/09/29 03:24:56  fwarmerdam
 * Don't blow a gasket if nSize is 0 in read or write.
 *
 * Revision 1.12  2005/09/11 18:32:42  fwarmerdam
 * Fixed up a few problems, works now
 *
 * Revision 1.11  2005/09/11 18:01:28  fwarmerdam
 * preliminary implementatin of fully virtualized large file api
 *
 */

#include "cpl_vsi_private.h"

#if defined(WIN32)

CPL_CVSID("$Id: cpl_vsil_win32.cpp,v 1.13 2005/09/29 03:24:56 fwarmerdam Exp $");

#include <windows.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <io.h>
#include <fcntl.h>
#include <direct.h>

#include "cpl_string.h"

/************************************************************************/
/* ==================================================================== */
/*                       VSIWin32FilesystemHandler                      */
/* ==================================================================== */
/************************************************************************/

class VSIWin32FilesystemHandler : public VSIFilesystemHandler 
{
public:
    virtual VSIVirtualHandle *Open( const char *pszFilename, 
                                    const char *pszAccess);
    virtual int      Stat( const char *pszFilename, VSIStatBufL *pStatBuf );
    virtual int      Unlink( const char *pszFilename );
    virtual int      Mkdir( const char *pszDirname, long nMode );
    virtual int      Rmdir( const char *pszDirname );
    virtual char   **ReadDir( const char *pszDirname );
};

/************************************************************************/
/* ==================================================================== */
/*                            VSIWin32Handle                            */
/* ==================================================================== */
/************************************************************************/

class VSIWin32Handle : public VSIVirtualHandle
{
  public:
    HANDLE       hFile;

    virtual int       Seek( vsi_l_offset nOffset, int nWhence );
    virtual vsi_l_offset Tell();
    virtual size_t    Read( void *pBuffer, size_t nSize, size_t nMemb );
    virtual size_t    Write( void *pBuffer, size_t nSize, size_t nMemb );
    virtual int       Eof();
    virtual int       Flush();
    virtual int       Close();
};

/************************************************************************/
/*                               Close()                                */
/************************************************************************/

int VSIWin32Handle::Close()

{
    return CloseHandle( hFile ) ? 0 : -1;
}

/************************************************************************/
/*                                Seek()                                */
/************************************************************************/

int VSIWin32Handle::Seek( vsi_l_offset nOffset, int nWhence )

{
    GUInt32       dwMoveMethod, dwMoveHigh;
    GUInt32       nMoveLow;
    LARGE_INTEGER li;

    switch(nWhence)
    {
        case SEEK_CUR:
            dwMoveMethod = FILE_CURRENT;
            break;
        case SEEK_END:
            dwMoveMethod = FILE_END;
            break;
        case SEEK_SET:
        default:
            dwMoveMethod = FILE_BEGIN;
            break;
    }

    li.QuadPart = nOffset;
    nMoveLow = li.LowPart;
    dwMoveHigh = li.HighPart;

    SetLastError( 0 );
    SetFilePointer(hFile, (LONG) nMoveLow, (PLONG)&dwMoveHigh,
                       dwMoveMethod);

    if( GetLastError() != NO_ERROR )
    {
#ifdef notdef
        LPVOID      lpMsgBuf = NULL;
        
        FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER 
                       | FORMAT_MESSAGE_FROM_SYSTEM
                       | FORMAT_MESSAGE_IGNORE_INSERTS,
                       NULL, GetLastError(), 
                       MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
                       (LPTSTR) &lpMsgBuf, 0, NULL );
 
        printf( "[ERROR %d]\n %s\n", GetLastError(), (char *) lpMsgBuf );
        printf( "nOffset=%u, nMoveLow=%u, dwMoveHigh=%u\n", 
                (GUInt32) nOffset, nMoveLow, dwMoveHigh );
#endif
        
        return -1;
    }
    else
        return 0;
}

/************************************************************************/
/*                                Tell()                                */
/************************************************************************/

vsi_l_offset VSIWin32Handle::Tell()

{
    LARGE_INTEGER   li;

    li.HighPart = 0;
    li.LowPart = SetFilePointer( hFile, 0, (PLONG) &(li.HighPart), 
                                 FILE_CURRENT );

    return li.QuadPart;
}

/************************************************************************/
/*                               Flush()                                */
/************************************************************************/

int VSIWin32Handle::Flush()

{
    FlushFileBuffers( hFile );
    return 0;
}

/************************************************************************/
/*                                Read()                                */
/************************************************************************/

size_t VSIWin32Handle::Read( void * pBuffer, size_t nSize, size_t nCount )

{
    DWORD       dwSizeRead;
    size_t      nResult;

    if( !ReadFile( hFile, pBuffer, (DWORD)(nSize*nCount), &dwSizeRead, NULL ) )
        nResult = 0;
    else if( nSize == 0 )
        nResult = 0;
    else
        nResult = dwSizeRead / nSize;

    return nResult;
}

/************************************************************************/
/*                               Write()                                */
/************************************************************************/

size_t VSIWin32Handle::Write( void * pBuffer, size_t nSize, size_t nCount )

{
    DWORD       dwSizeWritten;
    size_t      nResult;

    if( !WriteFile(hFile,pBuffer,(DWORD)(nSize*nCount),&dwSizeWritten,NULL) )
        nResult = 0;
    else if( nSize == 0)
        nResult = 0;
    else
        nResult = dwSizeWritten / nSize;

    return nResult;
}

/************************************************************************/
/*                                Eof()                                 */
/************************************************************************/

int VSIWin32Handle::Eof()

{
    vsi_l_offset       nCur, nEnd;

    nCur = Tell();
    Seek( 0, SEEK_END );
    nEnd = Tell();
    Seek( nCur, SEEK_SET );

    return (nCur == nEnd);
}

/************************************************************************/
/* ==================================================================== */
/*                       VSIWin32FilesystemHandler                      */
/* ==================================================================== */
/************************************************************************/

/************************************************************************/
/*                                Open()                                */
/************************************************************************/

VSIVirtualHandle *VSIWin32FilesystemHandler::Open( const char *pszFilename, 
                                                   const char *pszAccess )

{
    DWORD dwDesiredAccess, dwCreationDisposition;
    HANDLE hFile;

    if( strchr(pszAccess, '+') != NULL || strchr(pszAccess, 'w') != 0 )
        dwDesiredAccess = GENERIC_READ | GENERIC_WRITE;
    else
        dwDesiredAccess = GENERIC_READ;

    if( strstr(pszAccess, "w") != NULL )
        dwCreationDisposition = CREATE_ALWAYS;
    else
        dwCreationDisposition = OPEN_EXISTING;
        
    hFile = CreateFile( pszFilename, dwDesiredAccess, 
                        FILE_SHARE_READ | FILE_SHARE_WRITE, 
                        NULL, dwCreationDisposition, 
                        (dwDesiredAccess == GENERIC_READ) ? 
                        FILE_ATTRIBUTE_READONLY : FILE_ATTRIBUTE_NORMAL, 
                        NULL );

    if( hFile == INVALID_HANDLE_VALUE )
    {
        return NULL;
    }
    else
    {
        VSIWin32Handle *poHandle = new VSIWin32Handle;
        
        poHandle->hFile = hFile;
        
        return poHandle;
    }
}

/************************************************************************/
/*                                Stat()                                */
/************************************************************************/

int VSIWin32FilesystemHandler::Stat( const char * pszFilename, 
                                     VSIStatBufL * pStatBuf )

{
    return( stat( pszFilename, pStatBuf ) );
}

/************************************************************************/
/*                               Unlink()                               */
/************************************************************************/

int VSIWin32FilesystemHandler::Unlink( const char * pszFilename )

{
    return unlink( pszFilename );
}

/************************************************************************/
/*                               Mkdir()                                */
/************************************************************************/

int VSIWin32FilesystemHandler::Mkdir( const char * pszPathname,
                                      long nMode )

{
    (void) nMode;
    return mkdir( pszPathname );
}

/************************************************************************/
/*                               Rmdir()                                */
/************************************************************************/

int VSIWin32FilesystemHandler::Rmdir( const char * pszPathname )

{
    return rmdir( pszPathname );
}

/************************************************************************/
/*                              ReadDir()                               */
/************************************************************************/

char **VSIWin32FilesystemHandler::ReadDir( const char *pszPath )

{
    struct _finddata_t c_file;
    long    hFile;
    char    *pszFileSpec, **papszDir = NULL;

    if (strlen(pszPath) == 0)
        pszPath = ".";

    pszFileSpec = CPLStrdup(CPLSPrintf("%s\\*.*", pszPath));

    if ( (hFile = _findfirst( pszFileSpec, &c_file )) != -1L )
    {
        do
        {
            papszDir = CSLAddString(papszDir, c_file.name);
        } while( _findnext( hFile, &c_file ) == 0 );

        _findclose( hFile );
    }
    else
    {
        /* Should we generate an error???  
         * For now we'll just return NULL (at the end of the function)
         */
    }

    CPLFree(pszFileSpec);

    return papszDir;
}

/************************************************************************/
/*                     VSIInstallLargeFileHandler()                     */
/************************************************************************/

void VSIInstallLargeFileHandler()

{
    VSIFileManager::InstallHandler( string(""), 
                                    new VSIWin32FilesystemHandler );
}

#endif /* def WIN32 */


