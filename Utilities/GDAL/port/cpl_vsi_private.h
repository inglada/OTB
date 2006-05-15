/******************************************************************************
 * $Id$
 *
 * Project:  VSI Virtual File System
 * Purpose:  Private declarations for classes related to the virtual filesystem
 * Author:   Frank Warmerdam, warmerdam@pobox.com
 *
 ******************************************************************************
 * Copyright (c) 2005, Frank Warmerdam <warmerdam@pobox.com>
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
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************
 *
 * $Log: cpl_vsi_private.h,v $
 * Revision 1.4  2005/09/15 18:39:00  fwarmerdam
 * fixedup filemanager cleanup
 *
 * Revision 1.3  2005/09/13 15:17:52  dron
 * Unneeded semicolon removed.
 *
 * Revision 1.2  2005/09/13 15:16:44  dron
 * Added virtual destructor to VSIVirtualHandle and VSIFilesystemHandler classes.
 *
 * Revision 1.1  2005/09/11 18:00:49  fwarmerdam
 * New
 *
 */

#ifndef CPL_VSI_VIRTUAL_H_INCLUDED
#define CPL_VSI_VIRTUAL_H_INCLUDED

#include "cpl_vsi.h"

#include <map>
#include <vector>
#include <string>

/************************************************************************/
/*                           VSIVirtualHandle                           */
/************************************************************************/

class VSIVirtualHandle { 
  public:
    virtual int       Seek( vsi_l_offset nOffset, int nWhence ) = 0;
    virtual vsi_l_offset Tell() = 0;
    virtual size_t    Read( void *pBuffer, size_t nSize, size_t nMemb ) = 0;
    virtual size_t    Write( void *pBuffer, size_t nSize, size_t nMemb ) = 0;
    virtual int       Eof() = 0;
    virtual int       Flush() {return 0;}
    virtual int       Close() = 0;
    virtual           ~VSIVirtualHandle() { }
};

/************************************************************************/
/*                         VSIFilesystemHandler                         */
/************************************************************************/

class VSIFilesystemHandler {

public:
    virtual VSIVirtualHandle *Open( const char *pszFilename, 
                                    const char *pszAccess) = 0;
    virtual int      Stat( const char *pszFilename, VSIStatBufL *pStatBuf) = 0;
    virtual int      Unlink( const char *pszFilename ) 
        		{ errno=ENOENT; return -1; }
    virtual int      Mkdir( const char *pszDirname, long nMode ) 
        		{ errno=ENOENT; return -1; }
    virtual int      Rmdir( const char *pszDirname ) 
			{ errno=ENOENT; return -1; }
    virtual char   **ReadDir( const char *pszDirname ) 
			{ return NULL; }
    virtual          ~VSIFilesystemHandler() {}
};

/************************************************************************/
/*                            VSIFileManager                            */
/************************************************************************/

class VSIFileManager 
{
private:
    VSIFilesystemHandler         *poDefaultHandler;
    std::map<std::string,VSIFilesystemHandler *>   oHandlers;

    VSIFileManager();

    static VSIFileManager *Get();

public:
    ~VSIFileManager();

    static VSIFilesystemHandler *GetHandler( const char * );
    static void                InstallHandler( std::string osPrefix, 
                                               VSIFilesystemHandler * );
    static void                RemoveHandler( std::string osPrefix );
};

#endif /* ndef CPL_VSI_VIRTUAL_H_INCLUDED */
