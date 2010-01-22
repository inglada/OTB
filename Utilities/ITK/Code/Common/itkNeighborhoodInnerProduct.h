/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNeighborhoodInnerProduct.h,v $
  Language:  C++
  Date:      $Date: 2009-02-06 20:53:12 $
  Version:   $Revision: 1.14 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkNeighborhoodInnerProduct_h
#define __itkNeighborhoodInnerProduct_h

#include "itkNeighborhoodIterator.h"
#include "itkNeighborhood.h"
#include "itkConstSliceIterator.h"
#include "itkImageBoundaryCondition.h"

namespace itk {
  
/** \class NeighborhoodInnerProduct
 *
 * This class defines the inner product operation between an
 * itk::Neighborhood and and itk::NeighborhoodOperator.  The
 * operator() method is overloaded to support various types of
 * neighborhoods as well as inner products with slices of
 * neighborhoods. The second template parameter allows you to set the
 * value type of the operator. The third templage parameter allows you
 * to set the value type used as the return type of the inner product
 * calculation. The operator value type defaults to the image pixel
 * type and the output value type defaults to the operator type.
 *
 * \ingroup Operators
 */

template<class TImage, class TOperator=ITK_TYPENAME TImage::PixelType, class TComputation=TOperator>
class ITK_EXPORT NeighborhoodInnerProduct
{
public:
  /** Standard typedefs */
  typedef NeighborhoodInnerProduct Self;

  /** Capture some typedefs from the template parameters. */
  typedef typename TImage::PixelType ImagePixelType;
  typedef TOperator                  OperatorPixelType;
  typedef TComputation               OutputPixelType;
  
  /** Capture some typedefs from the template parameters. */
  itkStaticConstMacro(ImageDimension, unsigned int, TImage::ImageDimension);

  /** Operator typedef */
  typedef Neighborhood<OperatorPixelType,
                      itkGetStaticConstMacro(ImageDimension)> OperatorType;

  typedef Neighborhood<ImagePixelType,
                       itkGetStaticConstMacro(ImageDimension)> NeighborhoodType;
  
  /** Reference oeprator. */
  OutputPixelType operator()(const std::slice &s,
                       const ConstNeighborhoodIterator<TImage> &it,
                       const OperatorType &op) const;
  OutputPixelType operator()(const ConstNeighborhoodIterator<TImage> &it,
                       const OperatorType &op) const
    {
    return this->operator()(std::slice(0, it.Size(), 1), it, op);
    }


  OutputPixelType operator()(const std::slice &s,
                       const NeighborhoodType &N,
                             const OperatorType &op) const;
  
  OutputPixelType operator()(const NeighborhoodType &N,
                       const OperatorType &op) const
    {
    return this->operator()(std::slice(0, N.Size(), 1), N, op);
    }

};

} // end namespace itk
  
// Define instantiation macro for this template.
#define ITK_TEMPLATE_NeighborhoodInnerProduct(_, EXPORT, x, y) namespace itk { \
  _(3(class EXPORT NeighborhoodInnerProduct< ITK_TEMPLATE_3 x >)) \
  namespace Templates { typedef NeighborhoodInnerProduct< ITK_TEMPLATE_3 x > \
                        NeighborhoodInnerProduct##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkNeighborhoodInnerProduct+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkNeighborhoodInnerProduct.txx"
#endif

/*
#ifndef ITK_MANUAL_INSTANTIATION
#include "itkNeighborhoodInnerProduct.txx"
#endif
*/

#endif
