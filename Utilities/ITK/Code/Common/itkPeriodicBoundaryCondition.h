/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkPeriodicBoundaryCondition.h,v $
  Language:  C++
  Date:      $Date: 2005/09/07 14:46:31 $
  Version:   $Revision: 1.4 $

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

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkPeriodicBoundaryCondition.txx"
#endif

#endif
