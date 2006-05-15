/******************************************************************************
 * $Id: cplstring.cpp,v 1.2 2005/09/15 00:29:11 fwarmerdam Exp $
 *
 * Project:  GDAL 
 * Purpose:  CPLString implementation.
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
 *****************************************************************************
 *
 * $Log: cplstring.cpp,v $
 * Revision 1.2  2005/09/15 00:29:11  fwarmerdam
 * fixed typo.
 *
 * Revision 1.1  2005/08/31 03:31:46  fwarmerdam
 * New
 *
 */

#include "cpl_string.h"

CPL_CVSID("$Id: cplstring.cpp,v 1.2 2005/09/15 00:29:11 fwarmerdam Exp $");

/*
 * The CPLString class is derived from std::string, so the vast majority 
 * of the implementation comes from that.  This module is just the extensions
 * we add. 
 */

/************************************************************************/
/*                               Printf()                               */
/************************************************************************/

CPLString &CPLString::Printf( const char *pszFormat, ... )

{
    va_list args;

    va_start( args, pszFormat );
    vPrintf( pszFormat, args );
    va_end( args );

    return *this;
}

/************************************************************************/
/*                              vPrintf()                               */
/************************************************************************/

CPLString &CPLString::vPrintf( const char *pszFormat, va_list args )

{
/* -------------------------------------------------------------------- */
/*      This implementation for platforms without vsnprintf() will      */
/*      just plain fail if the formatted contents are too large.        */
/* -------------------------------------------------------------------- */

#if !defined(HAVE_VSNPRINTF)
    char *pszBuffer = CPLMalloc(30000);
    if( vsprintf( pszBuffer, pszFormat, args) > 29998 )
    {
        CPLError( CE_Fatal, CPLE_AppDefined, 
                  "CPLString::vPrintf() ... buffer overrun." );
    }
    *this = pszBuffer;
    CPLFree( pszBuffer );

/* -------------------------------------------------------------------- */
/*      This should grow a big enough buffer to hold any formatted      */
/*      result.                                                         */
/* -------------------------------------------------------------------- */
#else
    char szModestBuffer[500];

    if( vsnprintf( szModestBuffer, sizeof(szModestBuffer), pszFormat, args )
        >= (int) sizeof(szModestBuffer)-1 )
    {
        int nWorkBufferSize = 2000;
        char *pszWorkBuffer = (char *) CPLMalloc(nWorkBufferSize);

        while( vsnprintf( pszWorkBuffer, nWorkBufferSize, pszFormat, args )
               >= nWorkBufferSize-1 )
        {
            nWorkBufferSize *= 4;
            pszWorkBuffer = (char *) CPLRealloc(pszWorkBuffer, 
                                                nWorkBufferSize );
        }
        *this = pszWorkBuffer;
        CPLFree( pszWorkBuffer );
    }
    else
    {
        *this = szModestBuffer;
    }
#endif

    return *this;
}
