/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkConvertPixelBuffer.h,v $
  Language:  C++
  Date:      $Date: 2009-08-11 12:45:11 $
  Version:   $Revision: 1.12 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkConvertPixelBuffer_h
#define __itkConvertPixelBuffer_h

#include "itkObject.h"

namespace itk
{
/**
 * \class ConvertPixelBuffer
 *  \brief Class to convert blocks of data from one type to another.
 *
 * ConvertPixelBuffer has a static method Convert().  It is used by
 * itkImageFileReader to convert from an unknown type at run-time to the
 * compile-time template pixel type in itkImageFileReader.  To work with
 * complex pixel types like RGB and RGBA a traits class is used.
 * OutputConvertTraits() is the traits class.  The default one used is
 * DefaultConvertPixelTraits.  
 *
 */
template <
  typename InputPixelType,
  typename OutputPixelType,
  class OutputConvertTraits
  >
class ConvertPixelBuffer
{
public:
  /** Determine the output data type. */
  typedef typename OutputConvertTraits::ComponentType OutputComponentType;

  /** General method converts from one type to another. */
  static void Convert(InputPixelType* inputData, 
                      int inputNumberOfComponents, 
                      OutputPixelType* outputData , size_t size);
  static void ConvertVectorImage(InputPixelType* inputData, 
                      int inputNumberOfComponents, 
                      OutputPixelType* outputData , size_t size);
  static void ConvertComplexVectorImageToVectorImage(std::complex<InputPixelType>* inputData,
                      int inputNumberOfComponents, 
                      OutputPixelType* outputData , size_t size);
  static void ConvertComplexVectorImageToVectorImageComplex(std::complex<InputPixelType>* inputData,
                        int inputNumberOfComponents,
                        OutputPixelType* outputData , size_t size);

  static void ConvertComplexToGray(std::complex<InputPixelType>* inputData, 
                      int inputNumberOfComponents, 
                      OutputPixelType* outputData , size_t size);

protected:
  /** Convert to Gray output. */
  /** Input values are cast to output values. */
  static void ConvertGrayToGray(InputPixelType* inputData, 
                                OutputPixelType* outputData , size_t size);
  /** Weights convert from linear RGB to CIE luminance assuming a
   *  modern monitor. See Charles Poynton's Colour FAQ
   *
   *  http://www.inforamp.net/~poynton/notes/colour_and_gamma/ColorFAQ.html */
  static void ConvertRGBToGray(InputPixelType* inputData, 
                               OutputPixelType* outputData , size_t size);

  /** Weights convert from linear RGB to CIE luminance assuming a
   *  modern monitor. Values are attentuated by the Alpha channel. See
   *  Charles Poynton's Colour FAQ 
   *  http://www.inforamp.net/~poynton/notes/colour_and_gamma/ColorFAQ.html */
  static void ConvertRGBAToGray(InputPixelType* inputData, 
                                OutputPixelType* outputData , size_t size);
  static void ConvertMultiComponentToGray(InputPixelType* inputData, 
                                          int inputNumberOfComponents,
                                          OutputPixelType* outputData , 
                                          size_t size);
  
  /** Convert to RGB output. */
  /** Each RGB output component is set the the
   * input Gray value. */
  static void ConvertGrayToRGB(InputPixelType* inputData, 
                               OutputPixelType* outputData , size_t size);
  /** Input values are cast component by component to output values. */
  static void ConvertRGBToRGB(InputPixelType* inputData, 
                              OutputPixelType* outputData , size_t size);
  /** Input values are attenuated by the Alpha channel. */
  static void ConvertRGBAToRGB(InputPixelType* inputData, 
                               OutputPixelType* outputData , size_t size);
  /** Conversion depends upon the number of components in the
   * input. If the number of input components is 2, the output
   * components are each set to the first input component attenuated
   * by the second input component. This assumes that a two input
   * pixel represents intensity and alpha. If the number of input
   * components is not 2, the first three output components a are set
   * to the first three input components. The remaining input
   * components are ignored. */
  static void ConvertMultiComponentToRGB(InputPixelType* inputData, 
                                         int inputNumberOfComponents,
                                         OutputPixelType* outputData , 
                                         size_t size);
    
  /** Convert to RGBA output. */
  static void ConvertGrayToRGBA(InputPixelType* inputData, 
                                OutputPixelType* outputData , size_t size);
  static void ConvertRGBToRGBA(InputPixelType* inputData, 
                               OutputPixelType* outputData , size_t size);
  static void ConvertRGBAToRGBA(InputPixelType* inputData, 
                                OutputPixelType* outputData , size_t size);
  static void ConvertMultiComponentToRGBA(InputPixelType* inputData, 
                                          int inputNumberOfComponents,
                                          OutputPixelType* outputData , 
                                          size_t size);

  /** Convert tensor output. */
  /** Each input is made into a 6 component symmetric pixel */
  static void ConvertTensor6ToTensor6(InputPixelType* inputData, 
                               OutputPixelType* outputData , size_t size);
  static void ConvertTensor9ToTensor6(InputPixelType* inputData, 
                               OutputPixelType* outputData , size_t size);

  /** Convertions related to complex */
  static void ConvertGrayToComplex(InputPixelType * inputData,
                                OutputPixelType * OutputData, size_t size);
  static void ConvertComplexToComplex(InputPixelType* inputData, 
                                OutputPixelType* outputData , size_t size);
  static void ConvertMultiComponentToComplex(InputPixelType* inputData, 
                                int inputNumberOfComponents,
                                OutputPixelType* outputData , size_t size);

private:
  ConvertPixelBuffer();
  ~ConvertPixelBuffer();
};
} //namespace ITK


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkConvertPixelBuffer.txx"
#endif

#endif // __itkConvertPixelBuffer_h
