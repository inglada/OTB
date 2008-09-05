/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBrains2HeaderFactory.h,v $
  Language:  C++
  Date:      $Date: 2007-12-03 13:13:07 $
  Version:   $Revision: 1.6 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBrains2HeaderFactory_h
#define __itkBrains2HeaderFactory_h

#include "itkBrains2HeaderBase.h"

namespace itk
{
/** 
 * \class Brains2HeaderFactory
 * \brief Create instances of Brains2Header objects using an object factory.
 */
class Brains2HeaderFactory
{
public:
  /** Standard class typedefs. */
  typedef Brains2HeaderFactory Self;
  typedef Self *               Pointer;
  typedef const Self *         ConstPointer;

  Brains2HeaderFactory();
  virtual ~Brains2HeaderFactory();
  /** Class Methods used to interface with the registered factories */

  /** Convenient typedefs. */
  typedef itk::Brains2HeaderBase::Pointer Brains2HeaderBasePointer;

  /** Create the appropriate Brains2Header depending on the particulars of the file. */
  static Brains2HeaderBasePointer CreateBrains2HeaderReader(const std::string & TypeID);
  static Brains2HeaderBasePointer CreateBrains2HeaderWriter(const std::string & TypeID);

protected:
  //virtual void PrintSelf(std::ostream& os, Indent indent) const;

private:
  Brains2HeaderFactory(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
};
} // end namespace itk
#endif
