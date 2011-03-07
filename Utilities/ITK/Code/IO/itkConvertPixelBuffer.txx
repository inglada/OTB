/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkConvertPixelBuffer.txx,v $
  Language:  C++
  Date:      $Date: 2009-11-22 19:35:57 $
  Version:   $Revision: 1.27 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkConvertPixelBuffer_txx
#define __itkConvertPixelBuffer_txx
#include "itkConvertPixelBuffer.h"

#include "itkRGBPixel.h"

namespace itk
{
template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::Convert(InputPixelType* inputData, 
          int inputNumberOfComponents, 
          OutputPixelType* outputData , size_t size)
{
  switch(OutputConvertTraits::GetNumberOfComponents())
    {
    // output number of components is 1
    case 1:
      {
      switch(inputNumberOfComponents)
        {
        case 1:
          ConvertGrayToGray(inputData, outputData, size);
          break;
        case 3:
          ConvertRGBToGray(inputData, outputData, size);
          break;
        case 4:
          ConvertRGBAToGray(inputData, outputData, size);
          break;
        default:
          ConvertMultiComponentToGray(inputData, inputNumberOfComponents,
                                      outputData, size);
          break;
        }
      break;
      }
      // handle the complex case 
    case 2 :
      {
      switch(inputNumberOfComponents)
        {
        case 1:
          ConvertGrayToComplex(inputData,outputData,size);
          break;
        case 2:
          ConvertComplexToComplex(inputData,outputData,size);
          break;
        default:
          ConvertMultiComponentToComplex(inputData,inputNumberOfComponents,outputData,size);
          break;
        }
      break;
      }

      // output number of components is 3 RGB
    case 3:
      {
      switch(inputNumberOfComponents)
        {
        case 1:
          ConvertGrayToRGB(inputData, outputData, size);
          break;
        case 3:
          ConvertRGBToRGB(inputData, outputData, size);
          break;
        case 4:
          ConvertRGBAToRGB(inputData, outputData, size);
          break;
        default:
          ConvertMultiComponentToRGB(inputData, inputNumberOfComponents,
                                     outputData, size);
        }
      break;
      }
      // output number of components is 4 RGBA
    case 4:
      {
      switch(inputNumberOfComponents)
        {
        case 1:
          ConvertGrayToRGBA(inputData, outputData, size);
          break;
        case 3:
          ConvertRGBToRGBA(inputData, outputData, size);
          break;
        case 4:
          ConvertRGBAToRGBA(inputData, outputData, size);
          break;
        default:
          ConvertMultiComponentToRGBA(inputData, inputNumberOfComponents,
                                      outputData, size);
        }
      break;
      }
    // output number of components is 6 (SymmetricSecondRankTensor)
    case 6:
      {
      switch(inputNumberOfComponents)
        {
        case 6:
          ConvertTensor6ToTensor6(inputData, outputData, size);
          break;
        case 9:
          ConvertTensor9ToTensor6(inputData, outputData, size);
          break;
        default:
          itkGenericExceptionMacro( "No conversion available from "
                                    << inputNumberOfComponents
                                    << " components to: 6 components");
          break;
        }
      break;
      }
    default:
    itkGenericExceptionMacro( "No conversion available from "
      << inputNumberOfComponents
      << " components to: " 
      <<  OutputConvertTraits::GetNumberOfComponents()
      << " components");
    break;
    }
}
  
  
template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertGrayToGray(InputPixelType* inputData, 
                    OutputPixelType* outputData , size_t size)
{
  InputPixelType* endInput = inputData + size;
  while(inputData != endInput)
    {
    OutputConvertTraits::SetNthComponent(0, *outputData++,
                                         static_cast<OutputComponentType>
                                         (*inputData));
    inputData++;
    }
}


template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertRGBToGray(InputPixelType* inputData, 
                   OutputPixelType* outputData , size_t size)
{   
  // Weights convert from linear RGB to CIE luminance assuming a
  // modern monitor.  See Charles Pontyon's Colour FAQ
  // http://www.inforamp.net/~poynton/notes/colour_and_gamma/ColorFAQ.html
  // NOTE: The scale factors are converted to whole numbers for precision

  InputPixelType* endInput = inputData + size*3;
  while(inputData != endInput)
    {
    OutputComponentType val = static_cast<OutputComponentType>( 
      (2125.0 * static_cast<OutputComponentType>(*inputData) +
       7154.0 * static_cast<OutputComponentType>(*(inputData+1)) +
       0721.0 * static_cast<OutputComponentType>(*(inputData+2))) / 10000.0 );
    inputData += 3;
    OutputConvertTraits::SetNthComponent(0, *outputData++, val);
    }
  
}


template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertRGBAToGray(InputPixelType* inputData, 
                    OutputPixelType* outputData , size_t size)
{   
  // Weights convert from linear RGB to CIE luminance assuming a
  // modern monitor.  See Charles Pontyon's Colour FAQ
  // http://www.inforamp.net/~poynton/notes/colour_and_gamma/ColorFAQ.html
  // NOTE: The scale factors are converted to whole numbers for
  // precision
  InputPixelType* endInput = inputData + size*4;
  while(inputData != endInput)
    {
    double tempval = 
      (
      (2125.0 * static_cast<double>(*inputData) +
       7154.0 * static_cast<double>(*(inputData+1)) +
       0721.0 * static_cast<double>(*(inputData+2))) / 10000.0
      ) * static_cast<double>(*(inputData+3));
    inputData += 4;
    OutputComponentType val = static_cast<OutputComponentType>( tempval );
    OutputConvertTraits::SetNthComponent(0, *outputData++, val);
    }
  
}

template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertMultiComponentToGray(InputPixelType* inputData, 
                              int inputNumberOfComponents,
                              OutputPixelType* outputData , size_t size)
{
  // 2 components assumed intensity and alpha
  if(inputNumberOfComponents == 2)
    {
    InputPixelType* endInput = inputData + size * 2;
    while(inputData != endInput)
      {
      OutputComponentType val =
        static_cast<OutputComponentType>(*inputData) *
        static_cast<OutputComponentType>(*(inputData+1));
      inputData += 2;
      OutputConvertTraits::SetNthComponent(0, *outputData++, val);
      }
    }
  // just skip the rest of the data
  else
    {
    // Weights convert from linear RGB to CIE luminance assuming a
    // modern monitor.  See Charles Pontyon's Colour FAQ
    // http://www.inforamp.net/~poynton/notes/colour_and_gamma/ColorFAQ.html
    // NOTE: The scale factors are converted to whole numbers for
    // precision
    ptrdiff_t diff = inputNumberOfComponents - 4;
    InputPixelType* endInput = inputData + size * (size_t)inputNumberOfComponents;
    while(inputData != endInput)
      {
      double tempval = 
        (
          (2125.0 * static_cast<double>(*inputData) +
           7154.0 * static_cast<double>(*(inputData+1)) +
           0721.0 * static_cast<double>(*(inputData+2))) / 10000.0
          ) * static_cast<double>(*(inputData+3));
      inputData += 4;
      OutputComponentType val = static_cast<OutputComponentType>( tempval );
      OutputConvertTraits::SetNthComponent(0, *outputData++, val);
      inputData += diff;
      }
    }
}

  
template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertGrayToRGB(InputPixelType* inputData, 
                   OutputPixelType* outputData , size_t size)
{
  InputPixelType* endInput = inputData + size;
  while(inputData != endInput)
    {
    OutputConvertTraits::SetNthComponent(0, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*inputData));
    OutputConvertTraits::SetNthComponent(1, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*inputData));
    OutputConvertTraits::SetNthComponent(2, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*inputData));
    inputData++;
    outputData++;
    }
}


template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertRGBToRGB(InputPixelType* inputData, 
                  OutputPixelType* outputData , size_t size)
{
  InputPixelType* endInput = inputData + size * 3;
  while(inputData != endInput)
    {
    OutputConvertTraits::SetNthComponent(0, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*inputData));
    OutputConvertTraits::SetNthComponent(1, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*(inputData+1)));
    OutputConvertTraits::SetNthComponent(2, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*(inputData+2)));
    inputData += 3;
    outputData++;
    }
}

template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertRGBAToRGB(InputPixelType* inputData, 
                   OutputPixelType* outputData , size_t size)
{  
  InputPixelType* endInput = inputData + size* 4;
  while(inputData != endInput)
    {
    OutputConvertTraits::SetNthComponent(0, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*inputData));
    OutputConvertTraits::SetNthComponent(1, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*(inputData+1)));
    OutputConvertTraits::SetNthComponent(2, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*(inputData+2)));
    inputData += 3;
    inputData++; // skip alpha
    outputData++;
    }
}

template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertMultiComponentToRGB(InputPixelType* inputData, 
                             int inputNumberOfComponents,
                             OutputPixelType* outputData , 
                             size_t size)
{
  // assume intensity alpha
  if(inputNumberOfComponents == 2)
    {
    InputPixelType* endInput = inputData + size * 2;
    while(inputData != endInput)
      {
      OutputComponentType val =  
        static_cast<OutputComponentType>(*inputData) * 
        static_cast<OutputComponentType>(*(inputData+1));
      inputData += 2;
      OutputConvertTraits::SetNthComponent(0, *outputData, val);
      OutputConvertTraits::SetNthComponent(1, *outputData, val);
      OutputConvertTraits::SetNthComponent(2, *outputData, val);
      outputData++;
      }
    }
  // just skip the rest of the data
  else
    {
    ptrdiff_t diff = inputNumberOfComponents - 3;
    InputPixelType* endInput = inputData + size * (size_t)inputNumberOfComponents;
    while(inputData != endInput)
      {
      OutputConvertTraits::SetNthComponent(0, *outputData, 
                                           static_cast<OutputComponentType>
                                           (*inputData));
      OutputConvertTraits::SetNthComponent(1, *outputData, 
                                           static_cast<OutputComponentType>
                                           (*(inputData+1)));
      OutputConvertTraits::SetNthComponent(2, *outputData, 
                                           static_cast<OutputComponentType>
                                           (*(inputData+2)));
      inputData += 3;
      inputData += diff;
      outputData++;
      }
    }
}


template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertGrayToRGBA(InputPixelType* inputData, 
                    OutputPixelType* outputData , size_t size)
  
{
  InputPixelType* endInput = inputData + size;
  while(inputData != endInput)
    {
    OutputConvertTraits::SetNthComponent(0, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*inputData));
    OutputConvertTraits::SetNthComponent(1, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*inputData));
    OutputConvertTraits::SetNthComponent(2, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*inputData));
    OutputConvertTraits::SetNthComponent(3, *outputData, 
                                         static_cast<OutputComponentType>
                                         (1));
    inputData++;
    outputData++;
    }
}

template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertRGBToRGBA(InputPixelType* inputData, 
                   OutputPixelType* outputData , size_t size)
{
  InputPixelType* endInput = inputData + size * 3;
  while(inputData != endInput)
    {
    OutputConvertTraits::SetNthComponent(0, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*inputData));
    OutputConvertTraits::SetNthComponent(1, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*(inputData+1)));
    OutputConvertTraits::SetNthComponent(2, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*(inputData+2)));
    OutputConvertTraits::SetNthComponent(3, *outputData, 
                                         static_cast<OutputComponentType>
                                         (1));
    inputData += 3;
    outputData++;
    }
}


template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertRGBAToRGBA(InputPixelType* inputData, 
                    OutputPixelType* outputData , size_t size)
{
  InputPixelType* endInput = inputData + size*4;
  while(inputData != endInput)
    {
    OutputConvertTraits::SetNthComponent(0, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*inputData));
    OutputConvertTraits::SetNthComponent(1, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*(inputData+1)));
    OutputConvertTraits::SetNthComponent(2, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*(inputData+2)));
    OutputConvertTraits::SetNthComponent(3, *outputData, 
                                         static_cast<OutputComponentType>
                                         (*(inputData+3)));
    inputData += 4;
    outputData++;
    }
}

template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertMultiComponentToRGBA(InputPixelType* inputData, 
                              int inputNumberOfComponents,
                              OutputPixelType* outputData , 
                              size_t size)
{
  // equal weights for 2 components??
  if(inputNumberOfComponents == 2)
    {
    InputPixelType* endInput = inputData + size * 2;
    while(inputData != endInput)
      { 
      OutputComponentType val = static_cast<OutputComponentType>(*inputData);
      OutputComponentType alpha = static_cast<OutputComponentType>(*(inputData+1));
      inputData += 2;
      OutputConvertTraits::SetNthComponent(0, *outputData, val);
      OutputConvertTraits::SetNthComponent(1, *outputData, val);
      OutputConvertTraits::SetNthComponent(2, *outputData, val);
      OutputConvertTraits::SetNthComponent(3, *outputData, alpha);
      }
    }
  else
    {
    ptrdiff_t diff = inputNumberOfComponents - 4;
    InputPixelType* endInput = inputData + size * (size_t)inputNumberOfComponents;
    while(inputData != endInput)
      {
      OutputConvertTraits::SetNthComponent(0, *outputData, 
                                           static_cast<OutputComponentType>
                                           (*inputData));
      OutputConvertTraits::SetNthComponent(1, *outputData, 
                                           static_cast<OutputComponentType>
                                           (*(inputData+1)));
      OutputConvertTraits::SetNthComponent(2, *outputData, 
                                           static_cast<OutputComponentType>
                                           (*(inputData+2)));
      OutputConvertTraits::SetNthComponent(3, *outputData, 
                                           static_cast<OutputComponentType>
                                           (*(inputData+3)));
      inputData += 4;
      inputData += diff;
      outputData++;
      }
    }
}

template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertTensor6ToTensor6(InputPixelType* inputData, 
          OutputPixelType* outputData , size_t size)
{
  for( size_t i=0; i< size; i++ )
    {
    OutputConvertTraits::SetNthComponent( 0, *outputData,
                         static_cast<OutputComponentType>(*inputData));
    OutputConvertTraits::SetNthComponent( 1, *outputData,
                         static_cast<OutputComponentType>(*(inputData+1)));
    OutputConvertTraits::SetNthComponent( 2, *outputData,
                         static_cast<OutputComponentType>(*(inputData+2)));
    OutputConvertTraits::SetNthComponent( 3, *outputData,
                         static_cast<OutputComponentType>(*(inputData+3)));
    OutputConvertTraits::SetNthComponent( 4, *outputData,
                         static_cast<OutputComponentType>(*(inputData+4)));
    OutputConvertTraits::SetNthComponent( 5, *outputData,
                         static_cast<OutputComponentType>(*(inputData+5)));
    ++outputData;
    inputData += 6;
    }
}


// Convert Grayscale to Complex
template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertGrayToComplex(InputPixelType* inputData, 
                   OutputPixelType* outputData , size_t size)
{
  InputPixelType* endInput = inputData + size;
  while(inputData != endInput)
    {
    OutputConvertTraits::SetNthComponent(0, *outputData, 
      static_cast<OutputComponentType>
      (*inputData));
    OutputConvertTraits::SetNthComponent(1, *outputData, 
      static_cast<OutputComponentType>
      (0.));
    inputData++;
    outputData++;
    }
}

template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertComplexToGray(std::complex<InputPixelType>* inputData, 
                      int itkNotUsed(inputNumberOfComponents),
                      OutputPixelType* outputData , size_t size)
{
  OutputPixelType dummy;
  std::complex<InputPixelType>* endInput = inputData + size;
  while(inputData != endInput)
    {
    OutputConvertTraits::SetNthComponent(0, *outputData, 
      static_cast<OutputPixelType> (SpecialCast(*inputData,dummy)));
    inputData++;
    outputData++;
    }
}


template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertComplexToComplex(InputPixelType* inputData, 
                    OutputPixelType* outputData , size_t size)
{
  InputPixelType* endInput = inputData + size*2;
  while(inputData != endInput)
    {
    OutputConvertTraits::SetNthComponent(
      0, *outputData, 
      static_cast<OutputComponentType>
      (*inputData));
    OutputConvertTraits::SetNthComponent(
      1, *outputData, 
      static_cast<OutputComponentType>
      (*(inputData+1)));
    inputData += 2;
    outputData++;
    }
}


template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertTensor9ToTensor6(InputPixelType* inputData, 
          OutputPixelType* outputData , size_t size)
{
  for( size_t i=0; i< size; i++ )
    {
    OutputConvertTraits::SetNthComponent( 0, *outputData, 
        static_cast <  OutputComponentType >( *inputData ));
    OutputConvertTraits::SetNthComponent( 1, *outputData, 
        static_cast <  OutputComponentType >( *(inputData+1) ));
    OutputConvertTraits::SetNthComponent( 2, *outputData, 
        static_cast <  OutputComponentType >( *(inputData+2) ));
    OutputConvertTraits::SetNthComponent( 3, *outputData, 
        static_cast <  OutputComponentType >( *(inputData+4) ));
    OutputConvertTraits::SetNthComponent( 4, *outputData, 
        static_cast <  OutputComponentType >( *(inputData+5) ));
    OutputConvertTraits::SetNthComponent( 5, *outputData, 
        static_cast <  OutputComponentType >( *(inputData+8) ));
    ++outputData;
    inputData += 9;
    }
}


template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertMultiComponentToComplex(InputPixelType* inputData, 
                              int inputNumberOfComponents,
                              OutputPixelType* outputData , 
                              size_t size)
{
  ptrdiff_t diff = inputNumberOfComponents - 2;
  InputPixelType* endInput = inputData + size * (size_t)inputNumberOfComponents;
  while(inputData != endInput)
    {
      OutputConvertTraits::SetNthComponent(0, *outputData, 
                static_cast<OutputComponentType>
                (*inputData));
      OutputConvertTraits::SetNthComponent(1, *outputData, 
                static_cast<OutputComponentType>
                (*(inputData+1)));
      inputData += 2;
      inputData += diff;
      outputData++;
    }
}


template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertVectorImage(InputPixelType* inputData, 
                     int inputNumberOfComponents, 
                     OutputPixelType* outputData , size_t size)
{
  size_t length = size* (size_t)inputNumberOfComponents;
  for( size_t i=0; i< length; i++ )
    {
    OutputConvertTraits::SetNthComponent( 0, *outputData, 
      static_cast <  OutputComponentType >( *inputData ));
    ++outputData;
    ++inputData;
    }
}

/* To be able to convert transparently*/

template<typename InputType, typename OutputType>
OutputType
SpecialCast(const std::complex<InputType>& in, const OutputType& itkNotUsed(dummy))
{
  typedef typename itk::NumericTraits<std::complex<InputType> >::RealType       RealType;
  typedef typename itk::NumericTraits<std::complex<InputType> >::ScalarRealType ScalarRealType;

  //RealType inReal   = static_cast<RealType>(in);
  RealType    inReal( static_cast<ScalarRealType>(in.real()), static_cast<ScalarRealType>(in.imag()) );

  return static_cast < OutputType >( vcl_abs(inReal) );
}

template<typename InputType, typename OutputType>
std::complex<OutputType>
SpecialCast(const std::complex<InputType>& in, const std::complex<OutputType>& itkNotUsed(dummy))
{
  typedef typename itk::NumericTraits<std::complex<InputType> >::RealType       RealType;
  typedef typename itk::NumericTraits<std::complex<InputType> >::ScalarRealType ScalarRealType;

  RealType    inReal( static_cast<ScalarRealType>(in.real()), static_cast<ScalarRealType>(in.imag()) );

  return static_cast < std::complex<OutputType> >( inReal );
}

template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertComplexVectorImageToVectorImage(std::complex<InputPixelType>* inputData,
                     int inputNumberOfComponents, 
                     OutputPixelType* outputData , size_t size)
{
  size_t length = size* (size_t)inputNumberOfComponents;
  for( size_t i=0; i< length/2; i++ )
    {
    OutputConvertTraits::SetNthComponent( 0, *outputData, (*inputData).real());
    ++outputData;
    OutputConvertTraits::SetNthComponent( 0, *outputData, (*inputData).imag());
    ++outputData;
    ++inputData;
    }
}

template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertComplexVectorImageToVectorImageComplex(std::complex<InputPixelType>* inputData,
                     int inputNumberOfComponents,
                     OutputPixelType* outputData , size_t size)
{
  size_t length = size* (size_t)inputNumberOfComponents;
  OutputPixelType dummy;
  for( size_t i=0; i< length; i++ )
    {
    OutputConvertTraits::SetNthComponent( 0, *outputData,
                                          SpecialCast(*inputData, dummy));
    ++outputData;
    ++inputData;
    }
}

}// end namespace itk

#endif
