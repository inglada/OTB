/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPeriodicBoundaryCondition.h,v $
  Language:  C++
  Date:      $Date: 2007/01/30 20:56:09 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkPeriodicBoundaryCondition_h
#define __itkPeriodicBoundaryCondition_h
#include "itkNeighborhood.h"
#include "itkImageBoundaryCondition.h"

namespace itk
{

/** \class PeriodicBoundaryCondition
 * \brief
 * A function object that determines values outside of image boundaries
 * according to periodic (wrap-around) conditions.
 *
 * The input to this function object is a neighborhood iterator.  This boundary
 * condition object is designed to be given as a template argument to a
 * NeighborhoodIterator or any of the NeighborhoodIterator subclasses.
 * 
 * \ingroup DataRepresentation
 * \ingroup ImageObjects
 */
template<class TImage>
class ITK_EXPORT  PeriodicBoundaryCondition
  : public ImageBoundaryCondition<TImage>
{
public:
  /** Standard class typedefs. */ 
  typedef PeriodicBoundaryCondition Self;
  typedef ImageBoundaryCondition<TImage> Superclass;
  
  /** Extract information from the image type. */
  typedef typename Superclass::PixelType PixelType;
  typedef typename Superclass::PixelPointerType PixelPointerType;
  typedef typename Superclass::IndexType IndexType;
  typedef typename Superclass::OffsetType OffsetType;
  typedef typename Superclass::NeighborhoodType NeighborhoodType;
  
  typedef typename Superclass::NeighborhoodAccessorFunctorType 
                                 NeighborhoodAccessorFunctorType;

  /** Extract information from the image type. */
  itkStaticConstMacro(ImageDimension, unsigned int,Superclass::ImageDimension);
  
  /** Default constructor. */
  PeriodicBoundaryCondition() {}

  /** Computes and returns a neighborhood of appropriate values from
   * neighborhood iterator data.. */
  virtual PixelType operator()(const OffsetType& point_index,
                               const OffsetType& boundary_offset,
                               const NeighborhoodType *data) const; 

  /** Computes and returns the appropriate pixel value from
   * neighborhood iterator data, using the functor. */
  virtual PixelType operator()(
      const OffsetType& point_index,
      const OffsetType& boundary_offset,
      const NeighborhoodType *data,
      const NeighborhoodAccessorFunctorType &neighborhoodAccessorFunctor) const;
};

} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_PeriodicBoundaryCondition(_, EXPORT, x, y) namespace itk { \
  _(1(class EXPORT PeriodicBoundaryCondition< ITK_TEMPLATE_1 x >)) \
  namespace Templates { typedef PeriodicBoundaryCondition< ITK_TEMPLATE_1 x > \
                                         PeriodicBoundaryCondition##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkPeriodicBoundaryCondition+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkPeriodicBoundaryCondition.txx"
#endif

#endif
