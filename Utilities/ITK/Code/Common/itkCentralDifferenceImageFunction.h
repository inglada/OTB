/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCentralDifferenceImageFunction.h,v $
  Language:  C++
  Date:      $Date: 2008-11-10 16:55:01 $
  Version:   $Revision: 1.35 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCentralDifferenceImageFunction_h
#define __itkCentralDifferenceImageFunction_h

#include "itkImageFunction.h"
#include "itkCovariantVector.h"
#include "itkImageBase.h"

namespace itk
{

/**
 * \class CentralDifferenceImageFunction
 * \brief Calculate the derivative by central differencing.
 *
 * This class is templated over the input image type and
 * the coordinate representation type (e.g. float or double).
 *
 * Possible improvements:
 * - the use of Neighborhood operators may improve efficiency.
 *
 * \ingroup ImageFunctions
 */
template <
  class TInputImage, 
  class TCoordRep = float >
class ITK_EXPORT CentralDifferenceImageFunction :
  public ImageFunction< TInputImage, 
                        CovariantVector<double, \
                        ::itk::GetImageDimension<TInputImage>::ImageDimension>, 
                        TCoordRep >
{
public:
  /** Dimension underlying input image. */
  itkStaticConstMacro(ImageDimension, unsigned int,
                      TInputImage::ImageDimension);

  /** Standard class typedefs. */
  typedef CentralDifferenceImageFunction Self;
  typedef ImageFunction<TInputImage,
                        CovariantVector<double, 
                        itkGetStaticConstMacro(ImageDimension)>,
                        TCoordRep>       Superclass;
  typedef SmartPointer<Self>             Pointer;
  typedef SmartPointer<const Self>       ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(CentralDifferenceImageFunction, ImageFunction);

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

  /** Evalulate the image derivative by central differencing at specified index.
   *
   *  No bounds checking is done.
   *  The point is assume to lie within the image buffer.
   *
   *  ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
  virtual OutputType EvaluateAtIndex( const IndexType& index ) const;
  
  /** Evalulate the image derivative by central differencing at non-integer 
   *  positions.
   *
   *  No bounds checking is done.
   *  The point is assume to lie within the image buffer.
   *
   *  ImageFunction::IsInsideBuffer() can be used to check bounds before
   * calling the method. */
   virtual OutputType Evaluate( const PointType& point ) const
    { 
    IndexType index;
    this->ConvertPointToNearestIndex( point, index );
    return this->EvaluateAtIndex( index ); 
    }
  virtual OutputType EvaluateAtContinuousIndex( 
    const ContinuousIndexType& cindex ) const
    { 
    IndexType index;
    this->ConvertContinuousIndexToNearestIndex( cindex, index );
    return this->EvaluateAtIndex( index ); 
    }
  
  /** The UseImageDirection flag determines whether image derivatives are
   * computed with respect to the image grid or with respect to the physical
   * space. When this flag is ON the derivatives are computed with respect to
   * the coordinate system of physical space. The difference is whether we take
   * into account the image Direction or not. The flag ON will take into
   * account the image direction and will result in an extra matrix
   * multiplication compared to the amount of computation performed when the
   * flag is OFF.
   * The default value of this flag is the same as the CMAKE option
   * ITK_IMAGE_BEHAVES_AS_ORIENTED_IMAGE (i.e ON by default when ITK_IMAGE_BEHAVES_AS_ORIENTED_IMAGE is ON,
   * and  OFF by default when ITK_IMAGE_BEHAVES_AS_ORIENTED_IMAGE is OFF).*/
  itkSetMacro( UseImageDirection, bool );
  itkGetMacro( UseImageDirection, bool );
  itkBooleanMacro( UseImageDirection );

protected:
  CentralDifferenceImageFunction();
  ~CentralDifferenceImageFunction(){};
  void PrintSelf(std::ostream& os, Indent indent) const;

private:
  CentralDifferenceImageFunction( const Self& ); //purposely not implemented
  void operator=( const Self& ); //purposely not implemented

  // flag to take or not the image direction into account
  // when computing the derivatives.
  bool m_UseImageDirection;

};

} // end namespace itk

// Define instantiation macro for this template.
#define ITK_TEMPLATE_CentralDifferenceImageFunction(_, EXPORT, x, y) namespace itk { \
  _(2(class EXPORT CentralDifferenceImageFunction< ITK_TEMPLATE_2 x >)) \
  namespace Templates { typedef CentralDifferenceImageFunction< ITK_TEMPLATE_2 x > \
                                            CentralDifferenceImageFunction##y; } \
  }

#if ITK_TEMPLATE_EXPLICIT
# include "Templates/itkCentralDifferenceImageFunction+-.h"
#endif

#if ITK_TEMPLATE_TXX
# include "itkCentralDifferenceImageFunction.txx"
#endif

#endif
