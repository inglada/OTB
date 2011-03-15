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
#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "itkFixedArray.h"
#include "otbSavitzkyGolayInterpolationFunctor.h"
#include "otbTimeSeries.h"

int otbSavitzkyGolayInterpolationFunctorTest(int argc, char* argv[])
{

  typedef float PixelType;
  typedef unsigned int DoYType;
  const unsigned int nbDates = 100;
  typedef itk::FixedArray< PixelType, nbDates > SeriesType;
  typedef itk::FixedArray< DoYType, nbDates > DatesType;
  const unsigned int Radius = 2;



  // Create a time series
  SeriesType inSeries;
  SeriesType weightSeries;
  DatesType doySeries;

  for(unsigned int i=0; i<nbDates; ++i)
    {
    inSeries[i] = vcl_cos(i/100.0);
    doySeries[i] = i;
    weightSeries[i] = 1;
    }

  // Set artifacts
  inSeries[nbDates/4] = 0.0;
  inSeries[nbDates/2] = 0.0;

  weightSeries[nbDates/4] = 10.0;
  weightSeries[nbDates/2] = 10.0;
  
  typedef otb::Functor::SavitzkyGolayInterpolationFunctor<Radius, SeriesType, DatesType, SeriesType> FunctorType;

  FunctorType f;
  f.SetWeights( weightSeries );
  f.SetDates( doySeries );

  SeriesType outSeries = f(inSeries);

  double interpolError = 0.0;

  for(unsigned int i=0; i<nbDates; ++i)
    {

    interpolError += vcl_fabs(outSeries[i]-inSeries[i]);

    }

  interpolError/=nbDates;

  if(interpolError > 0.1)
    return EXIT_FAILURE;
  
  return EXIT_SUCCESS;
}
