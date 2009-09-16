/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkChangeLabelLabelMapFilter.txx,v $
  Language:  C++
  Date:      $Date: 2009-07-20 19:05:50 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkChangeLabelLabelMapFilter_txx
#define __itkChangeLabelLabelMapFilter_txx

#include "itkChangeLabelLabelMapFilter.h"
#include "itkProgressReporter.h"
#include <deque>


namespace itk {

template <class TImage>
ChangeLabelLabelMapFilter<TImage>
::ChangeLabelLabelMapFilter()
{
}

template <class TImage>
void
ChangeLabelLabelMapFilter<TImage>
::SetChangeMap( const ChangeMapType & changeMap )
{
  if( m_MapOfLabelToBeReplaced != changeMap )
    {
    m_MapOfLabelToBeReplaced = changeMap;
    this->Modified();
    }
}

template <class TImage>
const typename ChangeLabelLabelMapFilter<TImage>::ChangeMapType &
ChangeLabelLabelMapFilter<TImage>
::GetChangeMap() const
{
  return m_MapOfLabelToBeReplaced;
}

template <class TImage>
void
ChangeLabelLabelMapFilter<TImage>
::SetChange( const PixelType & oldLabel, const PixelType & newLabel )
{
  if( m_MapOfLabelToBeReplaced.find( oldLabel ) == m_MapOfLabelToBeReplaced.end() ||
      m_MapOfLabelToBeReplaced[ oldLabel ] != newLabel )
    {
    m_MapOfLabelToBeReplaced[ oldLabel ] = newLabel;
    this->Modified();
    }
}

template <class TImage>
void
ChangeLabelLabelMapFilter<TImage>
::ClearChangeMap()
{
  if( !m_MapOfLabelToBeReplaced.empty() )
    {
    m_MapOfLabelToBeReplaced.clear();
    this->Modified();
    }
}

template <class TImage>
void
ChangeLabelLabelMapFilter<TImage>
::GenerateData()
{
  this->MoveLabelsToTemporaryArray();
  this->ChangeBackgroundIfNeeded();
  this->RestoreLabelObjectsAndChangeLabels();
}

template <class TImage>
void
ChangeLabelLabelMapFilter<TImage>
::MoveLabelsToTemporaryArray()
{
  // Allocate the output
  this->AllocateOutputs();

  ImageType * output = this->GetOutput();

  // Report the progress
  ProgressReporter progress( this, 0, m_MapOfLabelToBeReplaced.size() );
  
  // First remove the ones to change and store them elsewhere to process later
  this->m_LabelObjectsToBeRelabeled.clear();

  ChangeMapIterator pairToReplace = m_MapOfLabelToBeReplaced.begin();

  while( pairToReplace != m_MapOfLabelToBeReplaced.end() )
    {
    const PixelType labelToBeReplaced = pairToReplace->first;

    if( labelToBeReplaced != output->GetBackgroundValue() )
      {
      if( output->HasLabel( labelToBeReplaced ) )
        {
        this->m_LabelObjectsToBeRelabeled.push_back( output->GetLabelObject( labelToBeReplaced ) );
        output->RemoveLabel( labelToBeReplaced );
        }
      }

    progress.CompletedPixel();
    pairToReplace++;
    }
    
}

template <class TImage>
void
ChangeLabelLabelMapFilter<TImage>
::ChangeBackgroundIfNeeded()
{
  ImageType * output = this->GetOutput();

  // Check if the background is among the list of labels to relabel.
  ChangeMapIterator backgroundLabelItr = m_MapOfLabelToBeReplaced.find( output->GetBackgroundValue() );
  const bool backgroundLabelMustBeReplaced = ( backgroundLabelItr != m_MapOfLabelToBeReplaced.end() );

  // Then change the label of the background if needed
  if( backgroundLabelMustBeReplaced )
    {
    const PixelType newLabelForBackground = m_MapOfLabelToBeReplaced[ output->GetBackgroundValue() ];

    if(  newLabelForBackground !=  output->GetBackgroundValue() )
      {
      if( output->HasLabel( newLabelForBackground ) )
        {
        // we must have a background - remove that object
        output->RemoveLabel( newLabelForBackground );
        }
      output->SetBackgroundValue( newLabelForBackground );
      }
    }
}


template <class TImage>
void
ChangeLabelLabelMapFilter<TImage>
::RestoreLabelObjectsAndChangeLabels()
{
  ImageType * output = this->GetOutput();

  // Put the objects back in the map, with the updated label
  typedef typename VectorType::iterator   LabelObjectIterator;
  LabelObjectIterator labelObjectItr = this->m_LabelObjectsToBeRelabeled.begin();

  ProgressReporter progress( this, 0, 1 );

  while( labelObjectItr != this->m_LabelObjectsToBeRelabeled.end() )
    {
    LabelObjectType * labelObjectSource = *labelObjectItr;
    PixelType newLabel = m_MapOfLabelToBeReplaced[ labelObjectSource->GetLabel() ];

    // Ignore the label if it is the background
    if( newLabel != output->GetBackgroundValue() )
      {

      // If the new label already exists in the output, then merge them.
      if( output->HasLabel( newLabel ) )
        {

        // Add the content of the label object to the one already there
        LabelObjectType * labelObjectDestination = output->GetLabelObject( newLabel );

        typedef typename LabelObjectType::LineContainerType  LineContainerType;

        const LineContainerType & lineContainer = labelObjectSource->GetLineContainer();
      
        typename LineContainerType::const_iterator lineItr = lineContainer.begin();

        while( lineItr != lineContainer.end() )
          {
          labelObjectDestination->AddLine( *lineItr );
          ++lineItr;
          }

        // be sure to have the lines well organized
        labelObjectDestination->Optimize();
        }
      else
        {
        // just put the label object in the label map with the new label
        labelObjectSource->SetLabel( newLabel );
        output->AddLabelObject( labelObjectSource );
        }
      }
    
    // go to the next label object
    progress.CompletedPixel();
    labelObjectItr++;
    }
}


template <class TImage>
void
ChangeLabelLabelMapFilter<TImage>
::PrintSelf(std::ostream& os, Indent indent) const
{
  this->Superclass::PrintSelf(os, indent);

  ChangeMapIterator pairToReplace = m_MapOfLabelToBeReplaced.begin();

  typedef typename NumericTraits< PixelType >::PrintType   LabelPrintType;

  os << indent << "Labels to replace: " << std::endl;
  while( pairToReplace != m_MapOfLabelToBeReplaced.end() )
    {
    const PixelType oldLabel = pairToReplace->first;
    const PixelType newLabel = pairToReplace->second;

    os << indent;
    os << static_cast< LabelPrintType >( oldLabel ) << " -> ";
    os << static_cast< LabelPrintType >( newLabel ) << std::endl;

    ++pairToReplace;
    }
}

}// end namespace itk
#endif
