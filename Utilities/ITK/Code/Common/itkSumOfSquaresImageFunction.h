/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkSumOfSquaresImageFunction.h,v $
  Language:  C++
  Date:      $Date: 2009-03-03 15:09:43 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkSumOfSquaresImageFunction_h
#define __itkSumOfSquaresImageFunction_h

#include "itkImageFunction.h"
#include "itkNumericTraits.h"
#include "itkNeighborhood.h"

namespace itk
{

/**
 * \class SumOfSquaresImageFunction
 * \brief Calculate the sum of squares in the neighborhood of a pixel
 *
 * Calculate the sum of squares of the pixel values over the standard 8, 26, etc. connected
 * neighborhood.  This calculation uses a ZeroFluxNeumannBoundaryCondition.
 *
 * If called with a ContinuousIndex or Point, the calculation is performed
 * at the nearest neighbor.
 *
 * This class is templated over the input image type and the 
 * coordinate representation type (e.g. float or double ).
 *
 * \ingroup ImageFunctions
 */
template <class TInputImage, class TCoordRep = float >
class ITK_EXPORT SumOfSquaresImageFunction :
  public ImageFunction< TInputImage, ITK_TYPENAME NumericTraits<typename TInputImage::PixelType>::RealType,
    TCoordRep >
{
public:
  /** Standard class typedefs. */
  typedef SumOfSquaresImageFunction Self;
  typedef ImageFunction<TInputImage, ITK_TYPENAME NumericTraits<typename TInputImage::PixelType>::RealType,
    TCoordRep >                     Superclass;
  typedef SmartPointer<Self>        Pointer;
  typedef SmartPointer<const Self>  ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(SumOfSquaresImageFunction, ImageFunction);

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** InputImageType typedef support. */
  typedef TInputImage InputImageType;

  /** OutputType typdef support. */
  typedef typename Superclass::OutputType OutputType;

  /** Index typedef support. */
  typedef typename Superclass::IndexType IndexType;
  
  /** ContinuousIndex typedef support. */
  typedef typename Superclass::ContinuousIndexType ContinuousIndexType;

  /** Point typedef support. */
  typedef typename Superclass::PointType PointType;

  /** Dimension of the underlying image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      InputImageType::ImageDimension);

  /** Datatype used for the variance */
  typedef typename NumericTraits<typename InputImageType::PixelType>::RealType
      RealType;

  /** Evalulate the function at specified index */
  virtual RealType EvaluateAtIndex( const IndexType& index ) const;
  
  /** Evaluate the function at non-integer positions */
  virtual RealType Evaluate( const PointType& point ) const
    { 
    IndexType index;
    this->ConvertPointToNearestIndex( point, index );
    return this->EvaluateAtIndex( index ); 
    }
  virtual RealType EvaluateAtContinuousIndex( 
    const ContinuousIndexType& cindex ) const
    { 
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex( cindex, index );
    return this->EvaluateAtIndex( index );
    }

  /** Get/Set the radius of the neighborhood over which the
      statistics are evaluated */
  itkGetConstReferenceMacro( NeighborhoodRadius, unsigned int );

  void SetNeighborhoodRadius(unsigned int radius)
    {
    m_NeighborhoodRadius = radius;

    m_NeighborhoodSize = 1;
    long twoRPlus1 = 2*m_NeighborhoodRadius + 1;
    for (unsigned int i=0; i < ImageDimension; i++)
      {
      m_NeighborhoodSize *= twoRPlus1;
      }
    }

  itkGetConstReferenceMacro(NeighborhoodSize, unsigned int);

protected:
  SumOfSquaresImageFunction();
  ~SumOfSquaresImageFunction(){};
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  SumOfSquaresImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  unsigned int m_NeighborhoodRadius;
  unsigned int m_NeighborhoodSize;
};

} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_SumOfSquaresImageFunction(_, EXPORT, x, y) namespace itk { \
  _(2(class EXPORT SumOfSquaresImageFunction< ITK_TEMPLATE_2 x >)) \
  namespace Templates { typedef SumOfSquaresImageFunction< ITK_TEMPLATE_2 x > \
                                         SumOfSquaresImageFunction##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkSumOfSquaresImageFunction+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkSumOfSquaresImageFunction.txx"
#endif


#endif
