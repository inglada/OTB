/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
  See OTBCopyright.txt for details.


     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __MuellerToCovarianceImageFilter_h
#define __MuellerToCovarianceImageFilter_h

#include "otbUnaryFunctorImageFilter.h"

namespace otb
 {

namespace Functor {

/** \class otbMuellerToCovarianceFunctor
 * \brief Evaluate the  MLC image from the Mueller image
 *
 * Output value are:
 *   channel #0 : \f$ M_{11} + M_{22} + 2*M_{12} \f$ \\
 *   channel #1 : \f$ M_{11} - M_{22} \f$ \\
 *   channel #2 : \f$ M_{11} + M_{22} - 2*M_{12} \f$ \\
 *   channel #3 : \f$ M_{13} + M_{23} + i*(M_{14}+M_{24}) \f$ \\
 *   channel #4 : \f$ M_{3Coherency3} - M_{44} - 2*i*M_{34} \f$ \\
 *   channel #5 : \f$ M_{13} - M_{23} - i*(M_{14}-M_{24}) \f$ \\
 *
 * Where \f$ M_{ij} are the coefficients of the input Mueller matrix.
 * Input pixel must have 10 channels (one for each Mueller matrix coeffcients).
  * The order of the channels corresponds to :
 * \f$  \begin{pmatrix}
 * {channel #0 }&{channel #1 }&{channel #2 }&{channel #3 } \\
 * {channel #4 }&{channel #5 }&{channel #6 }&{channel #7 } \\
 * {channel #8 }&{channel #9 }&{channel #10}&{channel #11} \\
 * {channel #12}&{channel #13}&{channel #14}&{channel #15} \\
 * \end{pmatrix}
 *
 * The output image has 6 channels : the diagonal and the upper element of the reciprocal matrix.
 * Element are stored from left to right, line by line.
 *
 * \ingroup Functor
 * \ingroup SARPolarimetry
 *
 * \sa MuellerToCircularPolarisationImageFilter
 * \sa MuellerToPolarisationDegreeAndPowerImageFilter
 *
 */

template< class TInput, class TOutput>
class MuellerToCovarianceFunctor
{
public:
  typedef std::complex<double>                      ComplexType;
  typedef typename TOutput::ValueType               OutputValueType;
  
  
  inline TOutput operator()( const TInput & Mueller ) const
    {
      TOutput result;
      result.SetSize(m_NumberOfComponentsPerPixel);
      
      // Keep the upper diagonal of the matrix
      const double M11 =  static_cast<double>(Mueller[0]);
      const double M12 =  static_cast<double>(Mueller[1]);
      const double M13 =  static_cast<double>(Mueller[2]);
      const double M14 =  static_cast<double>(Mueller[3]);
      const double M22 =  static_cast<double>(Mueller[5]);
      const double M23 =  static_cast<double>(Mueller[6]);
      const double M24 =  static_cast<double>(Mueller[7]);
      const double M33 =  static_cast<double>(Mueller[10]);
      const double M34 =  static_cast<double>(Mueller[11]);
      const double M44 =  static_cast<double>(Mueller[15]);
      
      const ComplexType hhhh(M11+M22+2.*M12, 0.0);
      const ComplexType hvhv(M11-M22, 0.0);
      const ComplexType vvvv(M11+M22-2.*M12, 0.0);
      const ComplexType hhhv(M13+M23, -1.*(M14+M24));
      const ComplexType hhvv(M33-M44, -2.*M34);
      const ComplexType hvvv(M13-M23, -1.*(M14-M24));
      
      result[0] = static_cast<OutputValueType>( hhhh );
      result[1] = static_cast<OutputValueType>( 2.* hhhv );
      result[2] = static_cast<OutputValueType>( hhvv );
      result[3] = static_cast<OutputValueType>( 4.* hvhv );
      result[4] = static_cast<OutputValueType>( 2.* hvvv );
      result[5] = static_cast<OutputValueType>( vvvv );
      
      return result;
    }
  
  unsigned int GetOutputSize()
    {
     return m_NumberOfComponentsPerPixel;
    }
  
  /** Constructor */
  MuellerToCovarianceFunctor() : m_NumberOfComponentsPerPixel(6)  {}
  
  /** Destructor */
  virtual ~MuellerToCovarianceFunctor() {}
  
 private:
  unsigned int m_NumberOfComponentsPerPixel;
};
}
 
 
/** \class otbMuellerToCovarianceImageFilter
 * \brief Compute the MLC  image
 * from the Mueller image (16 real channels)
 */
template <class TInputImage, class TOutputImage, class TFunction = Functor::MuellerToCovarianceFunctor<
    ITK_TYPENAME TInputImage::PixelType, ITK_TYPENAME TOutputImage::PixelType> >
class ITK_EXPORT MuellerToCovarianceImageFilter :
   public UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction>
{
public:
   /** Standard class typedefs. */
   typedef MuellerToCovarianceImageFilter  Self;
   typedef UnaryFunctorImageFilter<TInputImage, TOutputImage, TFunction> Superclass;
   typedef itk::SmartPointer<Self>        Pointer;
   typedef itk::SmartPointer<const Self>  ConstPointer;

   /** Method for creation through the object factory. */
   itkNewMacro(Self);

   /** Runtime information support. */
   itkTypeMacro(MuellerToCovarianceImageFilter, UnaryFunctorImageFilter);


protected:
   MuellerToCovarianceImageFilter() {}
  virtual ~MuellerToCovarianceImageFilter() {}

private:
  MuellerToCovarianceImageFilter(const Self&); // purposely not implemented
  void operator=(const Self&);          // purposely not implemented
};
  
} // end namespace otb

#endif
