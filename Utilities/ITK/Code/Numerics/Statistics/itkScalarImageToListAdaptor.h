/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkScalarImageToListAdaptor.h,v $
  Language:  C++
  Date:      $Date: 2008-08-19 22:27:57 $
  Version:   $Revision: 1.7 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkScalarImageToListAdaptor_h
#define __itkScalarImageToListAdaptor_h

#include <typeinfo>

#include "itkImage.h"
#include "itkPixelTraits.h"
#include "itkImageToListAdaptor.h"
#include "itkSmartPointer.h"
#include "itkImageRegionIterator.h"
#include "itkFixedArray.h"
#include "itkMacro.h"

namespace itk{ 
namespace Statistics{

/** \class ScalarImageToListAdaptor
 *  \brief This class provides ListSampleBase interfaces to ITK Image
 *
 * After calling SetImage(Image::Pointer) method to plug in the image object,
 * users can use Sample interfaces to access Image data.
 * However, the resulting data are a list of measurement vectors. The type of
 * data is measurement vector. For example, if the pixel type of Image object 
 * is STL vector< float > and each pixel has two different types of 
 * measurements, intensity and gradient magnitude, this adaptor has
 * measurement vector of type ITK Point< float, 2>, and one element of the Point
 * is intensity and the other is gradient magnitude.
 *
 * There are two concepts of dimensions for this container. One is for Image 
 * object, and the other is for measurement vector dimension.
 * Only when using ITK Index to access data, the former concept is applicable
 * Otherwise, dimensions means dimensions of measurement vectors. 
 *
 * From the above example, there were two elements in a pixel and each pixel
 * provides [] operator for accessing its elements. However, in many cases,
 * The pixel might be a scalar value such as int or float. In this case,
 * The pixel doesn't support [] operator. To deal with this problem,
 * This class has two companion classes, ScalarAccessor and VectorAccessor.
 * If the pixel type is a scalar type, then you don't have change the third
 * template argument. If you have pixel type is vector one and supports
 * [] operator, then replace third argument with VectorAccessor
 *
 * \sa Sample, ListSampleBase
 */

template < class TImage >
class ITK_EXPORT ScalarImageToListAdaptor 
  : public ImageToListAdaptor< 
  TImage, 
  FixedArray< typename TImage::PixelType, 1 > >
{
public:
  typedef FixedArray< typename TImage::PixelType, 1 > MeasurementVectorType;

  /** Standard class typedefs */
  typedef ScalarImageToListAdaptor Self;
  typedef ImageToListAdaptor< TImage, MeasurementVectorType > Superclass;
  typedef SmartPointer< Self > Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(ScalarImageToListAdaptor, ListSampleBase);
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** the number of components in a measurement vector */
  itkStaticConstMacro(MeasurementVectorSize, unsigned int, 1);
  

  /** Superclass typedefs for Measurement vector, measurement, 
   * Instance Identifier, frequency, size, size element value */
  typedef typename Superclass::FrequencyType FrequencyType;
  typedef typename Superclass::MeasurementType MeasurementType;
  typedef typename Superclass::InstanceIdentifier InstanceIdentifier;
  typedef typename Superclass::MeasurementVectorSizeType MeasurementVectorSizeType;
  typedef MeasurementVectorType ValueType;

  virtual void SetMeasurementVectorSize( const MeasurementVectorSizeType s ) 
    {
    // Measurement vector size for this class is fixed as the pixel's 
    // dimension. This method should have no effect
    if( s!=1 )
      {
      itkExceptionMacro( << "Cannot set measurement vector size of "
          << " ScalarImageToListAdaptor to " << s );
      }
    }

  /** Length of each measurement vector = 1 */
  MeasurementVectorSizeType GetMeasurementVectorSize() const
    {
    return MeasurementVectorSize;
    } 
 
  const MeasurementVectorType & GetMeasurementVector(const InstanceIdentifier &id) const;

protected:
  ScalarImageToListAdaptor() 
    {  
    Superclass::SetMeasurementVectorSize( MeasurementVectorSize); 
    }

  virtual ~ScalarImageToListAdaptor() {}
  void PrintSelf(std::ostream& os, Indent indent) const;  

private:
  ScalarImageToListAdaptor(const Self&); //purposely not implemented
  void operator=(const Self&); //purposely not implemented
  mutable MeasurementVectorType m_TempVector;
}; // end of class ScalarImageToListAdaptor

} // end of namespace Statistics
} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkScalarImageToListAdaptor.txx"
#endif

#endif
