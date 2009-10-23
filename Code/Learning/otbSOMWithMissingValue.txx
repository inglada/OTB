/*=========================================================================

  Program:   ORFEO Toolbox
  Language:  C++
  Date:      $Date$
  Version:   $Revision$


  Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
See OTBCopyright.txt for details.

  Copyright (c) Institut Telecom ; Telecom bretagne. All rights reserved.
See ITCopyright.txt for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE,  See the above copyright notices for more information.

=========================================================================*/
#ifndef _otbSOMWithMissingValue_txx
#define _otbSOMWithMissingValue_txx

#include "otbSOMWithMissingValue.h"

#include "itkNumericTraits.h"
#include "itkNeighborhoodIterator.h"
#include "itkImageRegionIteratorWithIndex.h"
#include "itkContinuousIndex.h"
#include "itkRandomImageSource.h"
#include "otbMacro.h"
#include "itkImageRegionIterator.h"

namespace otb
{
/**
 * Update the output map with a new sample by including the case when some
 * components of this new sample may be missing.
 * \param sample The new sample to learn,
 * \param beta The learning coefficient,
 * \param radius The radius of the nieghbourhood.
 */
template < class TListSample, class TMap,
     class TSOMLearningBehaviorFunctor,
     class TSOMNeighborhoodBehaviorFunctor >
void
SOMWithMissingValue< TListSample, TMap, TSOMLearningBehaviorFunctor, TSOMNeighborhoodBehaviorFunctor >
::UpdateMap( const NeuronType& sample, double beta, SizeType& radius )
{
  // output map pointer
  MapPointerType map = this->GetOutput(0);

  // winner index in the map
  IndexType position = map->GetWinner(sample);
  NeuronType winner = map->GetPixel(position);

  // Local neighborhood definition
  typedef typename MapType::Superclass ImageMapType;
  typedef itk::NeighborhoodIterator< ImageMapType > NeighborhoodIteratorType;
  typename MapType::RegionType mapRegion = map->GetLargestPossibleRegion();
  NeighborhoodIteratorType it ( radius, map, mapRegion );

  // Here, the periodic update is achieved 'by hand' since
  // PeriodicBoundaryCondition does not allow to modifiy
  // VectorImage contents
  SizeType mapSize = mapRegion.GetSize();
  IndexType positionToUpdate;

  // Iterate over the neighborhood ot the winner neuron
  it.SetLocation( position );

  for (unsigned int i = 0; i < it.Size(); i++ )
  {
    typename NeighborhoodIteratorType::OffsetType offset = it.GetOffset(i);

    // The neighborhood is of elliptic shape
    double theDistance = itk::NumericTraits< double >::Zero;
    for (int j = 0; j < MapType::ImageDimension; j++ )
      theDistance += pow( static_cast<double>( offset[j] ), 2.0 )
              / pow( static_cast<double>( radius[j] ), 2.0 );

    if ( theDistance <= 1.0 )
    {
      for (int j = 0; j < MapType::ImageDimension; j++ )
      {
        int pos = offset[j] + position[j];
        positionToUpdate[j] = ( pos >= 0 ) ?
          pos % mapSize[j] :
          ( mapSize[j] - ( (-pos) % mapSize[j] ) ) % mapSize[j];
      }

      NeuronType tempNeuron = it.GetPixel(i);
      NeuronType newNeuron ( tempNeuron );

      double tempBeta = beta / ( 1.0 + theDistance );
      for(unsigned int j = 0; j < newNeuron.Size(); j++ )
      {
        if ( !DistanceType::IsMissingValue( sample[j] ) )
          newNeuron[j] += static_cast<typename NeuronType::ValueType>(
                    ( sample[j] - tempNeuron[j] ) * tempBeta );
      }
      map->SetPixel(positionToUpdate,newNeuron);
    }
  }
}

} // end iof namespace otb

#endif



