/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBloxBoundaryPointItem.h,v $
  Language:  C++
  Date:      $Date: 2007-01-30 20:56:07 $
  Version:   $Revision: 1.14 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBloxBoundaryPointItem_h
#define __itkBloxBoundaryPointItem_h

#include "itkCovariantVector.h"
#include "itkPoint.h"
#include "itkBloxItem.h"

namespace itk
{

/**
 * \class BloxBoundaryPointItem
 * \brief A boundary point, stored in a BloxPixel.
 * \ingroup ImageObjects
 *
 * */

template <unsigned int VImageDimension>
class ITK_EXPORT BloxBoundaryPointItem: public BloxItem
{
public:
  /** The type of vector used to store the position of the 
   *  BoundaryPointItem */
  typedef Point<double, VImageDimension> PositionType;
  
  /** The type of vector used to store the gradient of the 
   *  BoundaryPointItem */
  typedef CovariantVector<double, VImageDimension> GradientType;

  /** Set the position of the boundary point in physical space */
  void SetPhysicalPosition(PositionType physPos)
    {
    m_PhysicalPosition = physPos;
    }

  /** Get the position of the boundary point in physical space */
  PositionType GetPhysicalPosition()
    {
    return m_PhysicalPosition;
    }

  /** Set the gradient of the boundary point */
  void SetGradient(GradientType grad)
    {
    m_Gradient = grad;
    }

  /** Get the gradient of the boundary point */
  GradientType GetGradient()
    {
    return m_Gradient;
    }
  
  BloxBoundaryPointItem();
  ~BloxBoundaryPointItem();

private:

  /** The position of the boundary point in the coordinate system of the
   * physical image in which the boundary pixel was located */
  PositionType m_PhysicalPosition;

  /** The gradient of the boundary point (non-normalized) */
  GradientType m_Gradient;

};

} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_BloxBoundaryPointItem(_, EXPORT, x, y) namespace itk { \
  _(1(class EXPORT BloxBoundaryPointItem< ITK_TEMPLATE_1 x >)) \
  namespace Templates { typedef BloxBoundaryPointItem< ITK_TEMPLATE_1 x > \
                                            BloxBoundaryPointItem##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkBloxBoundaryPointItem+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkBloxBoundaryPointItem.txx"
#endif

#endif
