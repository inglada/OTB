/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkConvertPixelBuffer.txx,v $
  Language:  C++
  Date:      $Date: 2007/03/29 18:39:27 $
  Version:   $Revision: 1.20 $

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

// This file was modified by otb developpers
// Modifications are surrounded by // Added ... and // End of modifications comment block.


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
          OutputPixelType* outputData , int size)
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
      // handle the complex case (added by otb developpers)
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
      // end of otb modifications

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
    }
}
  
  
template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertGrayToGray(InputPixelType* inputData, 
                    OutputPixelType* outputData , int size)
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
                   OutputPixelType* outputData , int size)
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
       0721.0 * static_cast<OutputComponentType>(*(inputData+2))) / 10000.0   );
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
                    OutputPixelType* outputData , int size)
{   
  // Weights convert from linear RGB to CIE luminance assuming a
  // modern monitor.  See Charles Pontyon's Colour FAQ
  // http://www.inforamp.net/~poynton/notes/colour_and_gamma/ColorFAQ.html
  // NOTE: The scale factors are converted to whole numbers for
  // precision
  InputPixelType* endInput = inputData + size*4;
  while(inputData != endInput)
    {
    double tempval = (
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
                              OutputPixelType* outputData , int size)
{
  // 2 components assumed intensity and alpha
  if(inputNumberOfComponents == 2)
    {
    InputPixelType* endInput = inputData + size * 2;
    while(inputData != endInput)
      {
      OutputComponentType val =  static_cast<OutputComponentType>(*inputData) *
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
    int diff = inputNumberOfComponents - 4;
    InputPixelType* endInput = inputData + size*inputNumberOfComponents;
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
                   OutputPixelType* outputData , int size)
{
  InputPixelType* endInput = inputData + size;
  while(inputData != endInput)
    {
    OutputConvertTraits::SetNthComponent(
      0, *outputData, 
      static_cast<OutputComponentType>
      (*inputData));
    OutputConvertTraits::SetNthComponent(
      1, *outputData, 
      static_cast<OutputComponentType>
      (*inputData));
    OutputConvertTraits::SetNthComponent(
      2, *outputData, 
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
                  OutputPixelType* outputData , int size)
{
  InputPixelType* endInput = inputData + size * 3;
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
    OutputConvertTraits::SetNthComponent(
      2, *outputData, 
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
                   OutputPixelType* outputData , int size)
{  
  InputPixelType* endInput = inputData + size* 4;
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
    OutputConvertTraits::SetNthComponent(
      2, *outputData, 
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
                             int size)
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
      OutputConvertTraits::SetNthComponent(
        0, *outputData, val);
      OutputConvertTraits::SetNthComponent(
        1, *outputData, val);
      OutputConvertTraits::SetNthComponent(
        2, *outputData, val);
      outputData++;
      }
    }
  // just skip the rest of the data
  else
    {
    int diff = inputNumberOfComponents - 3;
    InputPixelType* endInput = inputData + size * inputNumberOfComponents;
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
      OutputConvertTraits::SetNthComponent(
        2, *outputData, 
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
                    OutputPixelType* outputData , int size)
  
{
  InputPixelType* endInput = inputData + size;
  while(inputData != endInput)
    {
    OutputConvertTraits::SetNthComponent(
      0, *outputData, 
      static_cast<OutputComponentType>
      (*inputData));
    OutputConvertTraits::SetNthComponent(
      1, *outputData, 
      static_cast<OutputComponentType>
      (*inputData));
    OutputConvertTraits::SetNthComponent(
      2, *outputData, 
      static_cast<OutputComponentType>
      (*inputData));
    OutputConvertTraits::SetNthComponent(
      3, *outputData, 
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
                   OutputPixelType* outputData , int size)
{
  InputPixelType* endInput = inputData + size * 3;
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
    OutputConvertTraits::SetNthComponent(
      2, *outputData, 
      static_cast<OutputComponentType>
      (*(inputData+2)));
    OutputConvertTraits::SetNthComponent(
      3, *outputData, 
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
                    OutputPixelType* outputData , int size)
{
  InputPixelType* endInput = inputData + size*4;
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
    OutputConvertTraits::SetNthComponent(
      2, *outputData, 
      static_cast<OutputComponentType>
      (*(inputData+2)));
    OutputConvertTraits::SetNthComponent(
      3, *outputData, 
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
                              int size)
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
      OutputConvertTraits::SetNthComponent(
        0, *outputData, val);
      OutputConvertTraits::SetNthComponent(
        1, *outputData, val);
      OutputConvertTraits::SetNthComponent(
        2, *outputData, val);
      OutputConvertTraits::SetNthComponent(
        3, *outputData, alpha);
      }
    }
  else
    {
    int diff = inputNumberOfComponents - 4;
    InputPixelType* endInput = inputData + size * inputNumberOfComponents;
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
      OutputConvertTraits::SetNthComponent(
        2, *outputData, 
        static_cast<OutputComponentType>
        (*(inputData+2)));
      OutputConvertTraits::SetNthComponent(
        3, *outputData, 
        static_cast<OutputComponentType>
        (*(inputData+3)));
      inputData += 4;
      inputData += diff;
      outputData++;
      }
    }
}

// Added by otb developpers 

template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertGrayToComplex(InputPixelType* inputData, 
                   OutputPixelType* outputData , int size)
{
  InputPixelType* endInput = inputData + size;
  while(inputData != endInput)
    {
    OutputConvertTraits::SetNthComponent(
      0, *outputData, 
      static_cast<OutputComponentType>
      (*inputData));
    OutputConvertTraits::SetNthComponent(
      1, *outputData, 
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
::ConvertComplexToComplex(InputPixelType* inputData, 
                    OutputPixelType* outputData , int size)
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
           class OutputConvertTraits
           >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertMultiComponentToComplex(InputPixelType* inputData, 
                              int inputNumberOfComponents,
                              OutputPixelType* outputData , 
                              int size)
{
  int diff = inputNumberOfComponents - 2;
  InputPixelType* endInput = inputData + size * inputNumberOfComponents;
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
      inputData += diff;
      outputData++;
    }
}

// End of modifications

template < typename InputPixelType,
           typename OutputPixelType,
           class OutputConvertTraits >
void
ConvertPixelBuffer<InputPixelType, OutputPixelType, OutputConvertTraits>
::ConvertVectorImage(InputPixelType* inputData, 
                     int inputNumberOfComponents, 
                     OutputPixelType* outputData , int size)
{
  unsigned long length = static_cast< unsigned long >(size* inputNumberOfComponents);
  for( unsigned long i=0; i< length; i++ )
    {
    OutputConvertTraits::SetNthComponent(
      0, *outputData, 
      static_cast <  OutputComponentType >( *inputData ));
    ++outputData;
    ++inputData;
    }
}

}// end namespace itk

#endif
