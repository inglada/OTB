//*******************************************************************
// Copyright (C) 2000 Intelligence Data Systems. 
//
// LICENSE: LGPL
//
// see top level LICENSE.txt
// 
// Author: Garrett Potts
// Description: Nitf support class
// 
//********************************************************************
// $Id: ossimNitfRpcBTag.h 9094 2006-06-13 19:12:40Z dburken $
#ifndef ossimNitfRpcBTag_HEADER
#define ossimNitfRpcBTag_HEADER

#include <ossim/support_data/ossimNitfRpcBase.h>

/**
 * The layout of RPC00B is the same as RPC00A.  The difference is how the
 * coefficients are used; hence, all the code is in the ossimNitfRpcBase class.
 */
class OSSIM_DLL ossimNitfRpcBTag : public ossimNitfRpcBase
{
public:
   ossimNitfRpcBTag();

   /** @return "RPC00B" as an ossimString. */
   virtual ossimString getRegisterTagName() const;

TYPE_DATA   
};

#endif
