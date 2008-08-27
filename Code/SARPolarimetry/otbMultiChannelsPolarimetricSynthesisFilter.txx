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
#ifndef __otbMultiChannelsPolarimetricSynthesisFilter_txx
#define __otbMultiChannelsPolarimetricSynthesisFilter_txx

#include "otbMultiChannelsPolarimetricSynthesisFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkProgressReporter.h"
#include <complex>

namespace otb
{

/**
 * Constructor
 */
template <class TInputImage, class TOutputImage, class TFunction >
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::MultiChannelsPolarimetricSynthesisFilter()
{
  this->SetNumberOfRequiredInputs( 1 );
  this->InPlaceOff();
  SetEmissionH(false);
  SetEmissionV(false);  
  SetArchitectureType(0);
  SetGain(1);  
}

 /** PolarimetricSynthesisFilter
   *
   *
 * \sa ProcessObject::GenerateOutputInformaton() 
 */
template <class TInputImage, class TOutputImage, class TFunction>
void 
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::GenerateOutputInformation()
{
  // do not call the superclass' implementation of this method since
  // this filter allows the input the output to be of different dimensions
 
  // get pointers to the input and output
  typename Superclass::OutputImagePointer      outputPtr = this->GetOutput();
  typename Superclass::InputImageConstPointer  inputPtr  = this->GetInput();

  if ( !outputPtr || !inputPtr)
    {
    return;
    }

  // Set the output image largest possible region.  Use a RegionCopier
  // so that the input and output images can be different dimensions.
  OutputImageRegionType outputLargestPossibleRegion;
  this->CallCopyInputRegionToOutputRegion(outputLargestPossibleRegion,
                                          inputPtr->GetLargestPossibleRegion());
  outputPtr->SetLargestPossibleRegion( outputLargestPossibleRegion );

  // Set the output spacing and origin
  const itk::ImageBase<Superclass::InputImageDimension> *phyData;

  phyData
    = dynamic_cast<const itk::ImageBase<Superclass::InputImageDimension>*>(this->GetInput());

  if (phyData)
    {
    // Copy what we can from the image from spacing and origin of the input
    // This logic needs to be augmented with logic that select which
    // dimensions to copy
    unsigned int i, j;
    const typename InputImageType::SpacingType&
      inputSpacing = inputPtr->GetSpacing();
    const typename InputImageType::PointType&
      inputOrigin = inputPtr->GetOrigin();
    const typename InputImageType::DirectionType&
      inputDirection = inputPtr->GetDirection();

    typename OutputImageType::SpacingType outputSpacing;
    typename OutputImageType::PointType outputOrigin;
    typename OutputImageType::DirectionType outputDirection;

    // copy the input to the output and fill the rest of the
    // output with zeros.
    for (i=0; i < Superclass::InputImageDimension; ++i)
      {
      outputSpacing[i] = inputSpacing[i];
      outputOrigin[i] = inputOrigin[i];
      for (j=0; j < Superclass::OutputImageDimension; j++)
        {
        if (j < Superclass::InputImageDimension)
          {
          outputDirection[j][i] = inputDirection[j][i];
          }
        else
          {
          outputDirection[j][i] = 0.0;          
          }
        }
      }
    for (; i < Superclass::OutputImageDimension; ++i)
      {
      outputSpacing[i] = 1.0;
      outputOrigin[i] = 0.0;
      for (j=0; j < Superclass::OutputImageDimension; j++)
        {
        if (j == i)
          {
          outputDirection[j][i] = 1.0;
          }
        else
          {
          outputDirection[j][i] = 0.0;          
          }
        }
      }

    // set the spacing and origin
    outputPtr->SetSpacing( outputSpacing );
    outputPtr->SetOrigin( outputOrigin );
    outputPtr->SetDirection( outputDirection );
    
    }
  else
    {
    // pointer could not be cast back down
    itkExceptionMacro(<< "otb::MultiChannelRAndNIRVegetationIndexImageFilter::GenerateOutputInformation "
                      << "cannot cast input to "
                      << typeid(itk::ImageBase<Superclass::InputImageDimension>*).name() );
    }
}


/**
 * ThreadedGenerateData Performs the pixel-wise addition
 */
template <class TInputImage, class TOutputImage, class TFunction  >
void
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::ThreadedGenerateData( const OutputImageRegionType &outputRegionForThread,
                        int threadId)
{
 
  InputImagePointer  inputPtr = this->GetInput();
  OutputImagePointer outputPtr = this->GetOutput(0);
  
  // Define the portion of the input to walk for this thread, using
  // the CallCopyOutputRegionToInputRegion method allows for the input
  // and output images to be different dimensions
  InputImageRegionType inputRegionForThread;
  this->CallCopyOutputRegionToInputRegion(inputRegionForThread, outputRegionForThread);

  // Define the iterators
  itk::ImageRegionConstIterator<TInputImage>  inputIt(inputPtr, inputRegionForThread);
  itk::ImageRegionIterator<TOutputImage> outputIt(outputPtr, outputRegionForThread);

  itk::ProgressReporter progress(this, threadId, outputRegionForThread.GetNumberOfPixels());

  inputIt.GoToBegin();
  outputIt.GoToBegin();

  // Computation with 4 channels
  switch (m_ArchitectureType)
  {
          case 0 :
                while( !inputIt.IsAtEnd() ) 
                {
                outputIt.Set( m_Gain * m_Functor( inputIt.Get()[0], inputIt.Get()[1], inputIt.Get()[2], inputIt.Get()[3] ) );
                ++inputIt;
                ++outputIt;
                progress.CompletedPixel();  // potential exception thrown here
                }
              break;  

          // With 3 channels : HH HV VV ou HH VH VV
          case 1 :
                while( !inputIt.IsAtEnd() ) 
                {
                outputIt.Set( m_Gain * m_Functor( inputIt.Get()[0], inputIt.Get()[1], inputIt.Get()[1], inputIt.Get()[2] ) );
                ++inputIt;
                ++outputIt;
                progress.CompletedPixel();  // potential exception thrown here
                }
              break;  
               
          // Only HH and HV are present                
          case 2 :
                while( !inputIt.IsAtEnd() ) 
                {
                outputIt.Set( m_Gain * m_Functor( inputIt.Get()[0], inputIt.Get()[1], 0, 0 ) );
                ++inputIt;
                ++outputIt;
                progress.CompletedPixel();  // potential exception thrown here
                }
              break;  
                                
          // Only VH and VV are present
          case 3 :
                while( !inputIt.IsAtEnd() ) 
                {
                outputIt.Set( m_Gain * m_Functor( 0, 0, inputIt.Get()[2], inputIt.Get()[3] ) );
                ++inputIt;
                ++outputIt;
                progress.CompletedPixel();  // potential exception thrown here
                }
              break;  
       
          default :
              itkExceptionMacro("Unknown architecture : Polarimetric synthesis is impossible !");
              return;
  }    
    
}

/**
 * Computation of the electromagnetic fields Ei Er
 */
template <class TInputImage, class TOutputImage, class TFunction  >
void
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::ComputeElectromagneticFields()
{
  ComplexArrayType AEi, AEr;
  
  /** Conversion coefficient Degre To Radian */
  double DTOR=M_PI/180;  
  double real,imag;
  
  real = vcl_cos(DTOR*m_PsiI)*vcl_cos(DTOR*m_KhiI);
  imag = -vcl_sin(DTOR*m_PsiI)*vcl_sin(DTOR*m_KhiI);
  ComplexType Ei0(real,imag);
  
  real = vcl_sin(DTOR*m_PsiI)*vcl_cos(DTOR*m_KhiI);
  imag = vcl_cos(DTOR*m_PsiI)*vcl_sin(DTOR*m_KhiI);
  ComplexType Ei1(real,imag);
  
  real = vcl_cos(DTOR*m_PsiR)*vcl_cos(DTOR*m_KhiR);
  imag = -vcl_sin(DTOR*m_PsiR)*vcl_sin(DTOR*m_KhiR);
  ComplexType Er0(real,imag);
  
  real = vcl_sin(DTOR*m_PsiR)*vcl_cos(DTOR*m_KhiR);
  imag = vcl_cos(DTOR*m_PsiR)*vcl_sin(DTOR*m_KhiR);
  ComplexType Er1(real,imag);
    
  AEi[0]=Ei0;
  AEi[1]=Ei1;
  AEr[0]=Er0;
  AEr[1]=Er1;

  this->SetEi(AEi);
  this->SetEr(AEr);  
      
}

/**
 * Determine the kind of architecture
 */
template <class TInputImage, class TOutputImage, class TFunction  >
void
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::DetermineArchitecture()
{

  int NumberOfImages = this->GetInput()->GetNumberOfComponentsPerPixel();

  if ( NumberOfImages == 4 )
        SetArchitectureType(0);
        
  if ( NumberOfImages == 3 ) 
        SetArchitectureType(1);

  if ( ( NumberOfImages == 2  ) &&
       GetEmissionH() && !GetEmissionV() )
        SetArchitectureType(2);

  if ( ( NumberOfImages == 2  ) &&
       !GetEmissionH() && GetEmissionV() )
        SetArchitectureType(3);        

  // std::cout<<"Architecture: "<<GetArchitectureType()<<std::endl;

}


/**
 * Verify and force the inputs, if only  2 or 3 channels are present
 */
template <class TInputImage, class TOutputImage, class TFunction  >
void
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::VerifyAndForceInputs()
{

  switch (m_ArchitectureType)
    {
          case 0 :
                //std::cout<<"Case HH HV VH VV"<<std::endl;      
                break;  

          // With 3 channels : HH HV VV ou HH VH VV
          case 1 :
                //std::cout<<"Case 3 channels !!"<<std::endl;
                break;
                
          // Only HH and HV are present                
          case 2 :
                //std::cout<<"Case HH HV !!"<<std::endl;      
                             
        	// Forcing KhiI=0 PsiI=0
                this->SetKhiI(0);
                this->SetPsiI(0);
                break;
                                
          // Only VH and VV are present
          case 3 :
                //std::cout<<"Case VH VV !!"<<std::endl;  
        
                // Forcing KhiI=0 PsiI=90          
                this->SetKhiI(0);
                this->SetPsiI(90);
                break;
                
          default :
                itkExceptionMacro("Unknown architecture : Polarimetric synthesis is impossible !");
                return;
    }
    
  if(GetMode()==1)ForceCoPolar();
  else if(GetMode()==2)ForceCrossPolar();    
        
}

/**
 * BeforeThreadedGenerateData
 */
template <class TInputImage, class TOutputImage, class TFunction  >
void
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::BeforeThreadedGenerateData()
{

  // First Part. Determine the kind of architecture of the input picture
  DetermineArchitecture();
  
  // Second Part. Verify and force the inputs
  VerifyAndForceInputs();   
  
  // Third Part. Estimation of the incident field Ei and the reflected field Er
  ComputeElectromagneticFields();

  Print();
  
}

/**
 * Force Copolar mode
 */
template <class TInputImage, class TOutputImage, class TFunction  >
void
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::ForceCoPolar()
{
        SetPsiR(m_PsiI);
        SetKhiR(m_KhiI);        
        //std::cout<<"PsiI: "<<m_PsiI<<std::endl;  SetMode(1);
}

/**
 * Force Crosspolar mode
 */
template <class TInputImage, class TOutputImage, class TFunction  >
void
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::ForceCrossPolar()
{
        SetPsiR(m_PsiI+90);
        SetKhiR(-m_KhiI);
        SetMode(2);        
}

/**
 * Printself
 */
template <class TInputImage, class TOutputImage, class TFunction  >
void
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::PrintSelf(std::ostream& os, itk::Indent indent) const
{
  this->Superclass::PrintSelf(os,indent);
}

/**
 * Print
 */
template <class TInputImage, class TOutputImage, class TFunction  >
void
MultiChannelsPolarimetricSynthesisFilter<TInputImage,TOutputImage,TFunction>
::Print()
{
  //std::cout<<"PsiI: "<<m_PsiI<<std::endl;
  //std::cout<<"KhiI: "<<m_KhiI<<std::endl;
  //std::cout<<"PsiR: "<<m_PsiR<<std::endl;
  //std::cout<<"KhiR: "<<m_KhiR<<std::endl;
  
  //std::cout<<"Ei0 im: "<<m_Ei[0].imag()<<std::endl;
  //std::cout<<"Ei0 re: "<<m_Ei[0].real()<<std::endl;
  //std::cout<<"Ei1 im: "<<m_Ei[1].imag()<<std::endl;
  //std::cout<<"Ei1 re: "<<m_Ei[1].real()<<std::endl;
  
  //std::cout<<"Er0 im: "<<m_Er[0].imag()<<std::endl;
  //std::cout<<"Er0 re: "<<m_Er[0].real()<<std::endl;
  //std::cout<<"Er1 im: "<<m_Er[1].imag()<<std::endl;
  //std::cout<<"Er1 re: "<<m_Er[1].real()<<std::endl;
}

} // end namespace otb

#endif
