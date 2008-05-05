/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBloxItem.h,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:29:02 $
  Version:   $Revision: 1.10 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBloxItem_h
#define __itkBloxItem_h

#include "itkWin32Header.h"

namespace itk
{

/**
 * \class BloxItem
 * \brief An entry in the BloxPixel linked list
 *
 * This class is do-nothing virtual class, designed to avoid
 * the necessity of templating BloxPixel over item type.
 * \ingroup ImageObjects
 * */

class ITKCommon_EXPORT BloxItem
{
public:
  BloxItem();
  virtual ~BloxItem();

};

} // end namespace itk

#endif
