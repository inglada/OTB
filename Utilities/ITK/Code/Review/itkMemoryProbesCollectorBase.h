/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMemoryProbesCollectorBase.h,v $
  Language:  C++
  Date:      $Date: 2008-04-05 16:50:25 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMemoryProbesCollectorBase_h
#define __itkMemoryProbesCollectorBase_h

#include "itkMacro.h"
#include "itkMemoryProbe.h"
#include "itkProbesCollectorBase.h"

namespace itk
{

/** \class MemoryProbesCollectorBase
 *  \brief Class for aggregating a set of memory probes. 
 *
 *  This class defines a set of MemoryProbes and assign names to them.
 *  The user can start and stop each one of the probes by addressing them by name.
 *  
 *  \sa MemoryProbe
 *
 */
class ITK_EXPORT MemoryProbesCollectorBase :public ProbesCollectorBase<MemoryProbe>
{
public: 
  virtual ~MemoryProbesCollectorBase(){}
};

} // end namespace itk

#endif // __itkMemoryProbesCollectorBase_h
