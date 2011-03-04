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
#include "itkExceptionObject.h"

#include "otbSinclairToCoherencyFunctor.h"
#include "itkVariableLengthVector.h"

int otbSinclairToCoherencyFunctor(int argc, char * argv[])
{
  typedef std::complex<double>                   ComplexType;
  typedef itk::VariableLengthVector<ComplexType> OutputType;

  typedef otb::Functor::SinclairToCoherencyFunctor<ComplexType, ComplexType, ComplexType, ComplexType, OutputType >         FunctorType;

  OutputType  result(10);
  FunctorType funct;
  OutputType outputFunct;
  
  result[0] = ComplexType(25., 0.);
  result[1] = ComplexType( 0., -15.);
  result[2] = ComplexType(25., 0.);
  result[3] = ComplexType(-5., 0.);
  result[4] = ComplexType( 9., 0.);
  result[5] = ComplexType( 0., 15.);
  result[6] = ComplexType( 0., -3.);
  result[7] = ComplexType(25., 0.);
  result[8] = ComplexType(-5., 0.);
  result[9] = ComplexType( 1., 0.);

  outputFunct = funct.operator ()( ComplexType(1., 4.), ComplexType(2.,3.), ComplexType(3.,2.), ComplexType(4.,1.) );

  if( vcl_abs(result[0]-outputFunct[0]) > 1e-10 ||
      vcl_abs(result[1]-outputFunct[1]) > 1e-10 ||
      vcl_abs(result[2]-outputFunct[2]) > 1e-10 ||
      vcl_abs(result[3]-outputFunct[3]) > 1e-10 ||
      vcl_abs(result[4]-outputFunct[4]) > 1e-10 ||
      vcl_abs(result[5]-outputFunct[5]) > 1e-10 ||
      vcl_abs(result[6]-outputFunct[6]) > 1e-10 ||
      vcl_abs(result[7]-outputFunct[7]) > 1e-10 ||
      vcl_abs(result[8]-outputFunct[8]) > 1e-10 ||
      vcl_abs(result[9]-outputFunct[9]) > 1e-10)
  {
    std::cout<<"Test gives :"<<std::endl;
    std::cout<<outputFunct<<std::endl;
    std::cout<<"Wanted results are :"<<std::endl;
    std::cout<<result<<std::endl;

    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
