/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBloxPixel.h,v $
  Language:  C++
  Date:      $Date: 2007/02/05 13:35:52 $
  Version:   $Revision: 1.15 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBloxPixel_h
#define __itkBloxPixel_h

#include <list>

#include "itkBloxItem.h"

namespace itk
{

/**
 * \class BloxPixel
 * \brief Holds a linked list of BloxItem's
 *
 * itk::BloxPixel is a specialized "value added" version of the basic STL list
 * intended as a base class for all pixels stored in itk::BloxImage derived 
 * classes.
 * A particular type of itk::BloxImage is fully specialized by setting the type
 * of itk::BloxPixel that it holds, so in some sense this is the most important
 * class in the blox hierarchy.
 *
 * It is assumed that particular itk::BloxPixel derived types will add 
 * functionality to this base class; for example, eigenanalysis of core atom 
 * populations in itk::BloxCoreAtomPixel
 *
 * \ingroup ImageObjects
 */
template <typename TItemType>
class BloxPixel : public std::list<TItemType*>
{
public:

  /** Delete all entries in the list, then clear the list. */
  void DeleteListEntries();

  /** Get the number of items stored in the blox. */
  unsigned long int GetSize()
    {return static_cast<unsigned long>( this->size() ); }

  BloxPixel();
  ~BloxPixel();
};


} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_BloxPixel(_, EXPORT, x, y) namespace itk { \
  _(1(class EXPORT BloxPixel< ITK_TEMPLATE_1 x >)) \
  namespace Templates { typedef BloxPixel< ITK_TEMPLATE_1 x > \
                               BloxPixel##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkBloxPixel+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkBloxPixel.txx"
#endif

#endif
