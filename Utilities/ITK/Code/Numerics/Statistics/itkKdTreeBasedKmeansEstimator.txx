/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkKdTreeBasedKmeansEstimator.txx,v $
  Language:  C++
  Date:      $Date: 2009-03-04 15:23:51 $
  Version:   $Revision: 1.18 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkKdTreeBasedKmeansEstimator_txx
#define __itkKdTreeBasedKmeansEstimator_txx

#include "itkKdTreeBasedKmeansEstimator.h"
#include "itkStatisticsAlgorithm.h"

namespace itk {
namespace Statistics {

template< class TKdTree >
KdTreeBasedKmeansEstimator< TKdTree >
::KdTreeBasedKmeansEstimator()
{
  m_CentroidPositionChangesThreshold = 0.0;
  m_KdTree = 0;
  m_UseClusterLabels = false;
  m_MaximumIteration = 100;
  m_DistanceMetric = EuclideanDistance< ParameterType >::New();

  m_CentroidPositionChanges = 0.0;
  m_TempVertex.Fill( 0.0 );
  m_CurrentIteration = 0;
  m_MeasurementVectorSize = 0;
}

template< class TKdTree >
void
KdTreeBasedKmeansEstimator< TKdTree >
::PrintSelf( std::ostream& os, Indent indent ) const
{
  Superclass::PrintSelf(os,indent);

  
  os << indent << "Current Iteration: "
     << m_CurrentIteration << std::endl;
  os << indent << "Maximum Iteration: "
     << m_MaximumIteration << std::endl;

  os << indent << "Sum of Centroid Position Changes: "
     << m_CentroidPositionChanges << std::endl;
  os << indent << "Threshold for the Sum of Centroid Position Changes: "
     << m_CentroidPositionChangesThreshold << std::endl;

  os << indent << "Kd Tree:";
  if ( m_KdTree.IsNotNull() )
    {
    os << m_KdTree << std::endl;
    }
  else
    {
    os << "not set." << std::endl;
    }

  os << indent << "Distance Metric: " << m_DistanceMetric << std::endl;
  os << indent << "Parameters: " << m_Parameters << std::endl;
  os << indent << "Temp Vertex: " << m_TempVertex << std::endl;
  os << indent << "MeasurementVectorSize: " << m_MeasurementVectorSize << std::endl;
}


template< class TKdTree >
double 
KdTreeBasedKmeansEstimator< TKdTree >
::GetSumOfSquaredPositionChanges(InternalParametersType &previous, 
                                 InternalParametersType &current) 
{
  double temp;
  double sum = 0.0;
  unsigned int i;
    

  for (i = 0; i < (unsigned int)previous.size(); i++)
    {
    temp = m_DistanceMetric->Evaluate(previous[i], current[i]);
    sum += temp;
    }
  return sum;
}

template< class TKdTree >
inline int 
KdTreeBasedKmeansEstimator< TKdTree >
::GetClosestCandidate(ParameterType &measurements, 
                      std::vector< int > &validIndexes)
{

  int closest = 0;
  double closestDistance = NumericTraits< double >::max();
  double tempDistance;
  std::vector< int >::iterator iter = validIndexes.begin();
  while (iter != validIndexes.end())
    {
    tempDistance = 
      m_DistanceMetric->Evaluate(m_CandidateVector[*iter].Centroid,
                                 measurements);
    if (tempDistance < closestDistance)
      {
      closest = *iter;
      closestDistance = tempDistance;
      }
    ++iter;
    }
  return closest;
}

template< class TKdTree >
inline bool
KdTreeBasedKmeansEstimator< TKdTree >
::IsFarther(ParameterType &pointA,
            ParameterType &pointB,
            MeasurementVectorType &lowerBound,
            MeasurementVectorType &upperBound) 
{
  // calculates the vertex of the Cell bounded by the lowerBound
  // and the upperBound
  for (unsigned int i = 0; i < m_MeasurementVectorSize; i++)
    {
    if ((pointA[i] - pointB[i]) < 0.0)
      {
      m_TempVertex[i] = lowerBound[i];
      }
    else
      {
      m_TempVertex[i] = upperBound[i];
      }
    }

  if (m_DistanceMetric->Evaluate(pointA, m_TempVertex) >= 
      m_DistanceMetric->Evaluate(pointB, m_TempVertex))
    {
    return true;
    }

  return false;
}

template< class TKdTree >
inline void
KdTreeBasedKmeansEstimator< TKdTree >
::Filter(KdTreeNodeType* node, 
         std::vector< int > validIndexes,
         MeasurementVectorType &lowerBound,
         MeasurementVectorType &upperBound)
{
  unsigned int i, j;
  typename TKdTree::InstanceIdentifier tempId;
  int closest;
  ParameterType individualPoint;
  MeasurementVectorTraits::SetLength( individualPoint, this->m_MeasurementVectorSize );
  
  if ( node->IsTerminal() )
    {
    // terminal node
    if (node == m_KdTree->GetEmptyTerminalNode())
      {
      // empty node
      return;
      }

    for (i = 0; i < (unsigned int)node->Size(); i++)
      {
      tempId = node->GetInstanceIdentifier(i);
      this->GetPoint(individualPoint,
                     m_KdTree->GetMeasurementVector(tempId));
      closest = 
        this->GetClosestCandidate(individualPoint, validIndexes);
      for (j = 0; j < m_MeasurementVectorSize; j++)
        {
        m_CandidateVector[closest].WeightedCentroid[j] +=
          individualPoint[j];
        }
      m_CandidateVector[closest].Size += 1;
      if ( m_GenerateClusterLabels )
        {
        m_ClusterLabels[tempId] = closest;
        }
      }
    }
  else
    {
    CentroidType centroid; 
    CentroidType weightedCentroid;
    ParameterType closestPosition;
    node->GetWeightedCentroid(weightedCentroid);
    node->GetCentroid(centroid);


    closest = 
      this->GetClosestCandidate(centroid, validIndexes);
    closestPosition = m_CandidateVector[closest].Centroid;
    std::vector< int >::iterator iter = validIndexes.begin();

    while (iter != validIndexes.end())
      {
      if (*iter != closest &&
          this->IsFarther(m_CandidateVector[*iter].Centroid,
                          closestPosition,
                          lowerBound, upperBound))
        {
        iter = validIndexes.erase(iter);
        continue;
        }

      if (iter != validIndexes.end())
        {
        ++iter;
        }
      }
    

    if (validIndexes.size() == 1)
      {
      for (j = 0; j < m_MeasurementVectorSize; j++)
        {
        m_CandidateVector[closest].WeightedCentroid[j] += 
          weightedCentroid[j];
        }
      m_CandidateVector[closest].Size += node->Size();
      if ( m_GenerateClusterLabels )
        {
        this->FillClusterLabels(node, closest);
        }
      }
    else
      {
      unsigned int partitionDimension; 
      MeasurementType partitionValue;
      MeasurementType tempValue;
      node->GetParameters(partitionDimension, partitionValue);

      tempValue = upperBound[partitionDimension];
      upperBound[partitionDimension] = partitionValue;
      this->Filter(node->Left(), validIndexes, 
                   lowerBound, upperBound);
      upperBound[partitionDimension] = tempValue;

      tempValue = lowerBound[partitionDimension];
      lowerBound[partitionDimension] = partitionValue;
      this->Filter(node->Right(), validIndexes,
                   lowerBound, upperBound);
      lowerBound[partitionDimension] = tempValue;
      }
    }
}

template< class TKdTree >
void
KdTreeBasedKmeansEstimator< TKdTree >
::FillClusterLabels(KdTreeNodeType* node, int closestIndex)
{
  unsigned int i;

  if ( node->IsTerminal() )
    {
    // terminal node
    if (node == m_KdTree->GetEmptyTerminalNode())
      {
      // empty node
      return;
      }

    for (i = 0; i < (unsigned int)node->Size(); i++)
      {
      m_ClusterLabels[node->GetInstanceIdentifier(i)] = closestIndex;
      }
    }
  else
    {
    this->FillClusterLabels(node->Left(), closestIndex);
    this->FillClusterLabels(node->Right(), closestIndex);
    }  
}

template< class TKdTree >
void
KdTreeBasedKmeansEstimator< TKdTree >
::CopyParameters(ParametersType &source, InternalParametersType &target)
{
  unsigned int i, j;
  int index = 0;
  for (i = 0; i < (unsigned int)(source.size() / m_MeasurementVectorSize); i++)
    {
    for (j = 0; j < m_MeasurementVectorSize; j++)
      {
      target[i][j] = source[index];
      ++index;
      }
    }
}

template< class TKdTree >
void
KdTreeBasedKmeansEstimator< TKdTree >
::CopyParameters(InternalParametersType &source, ParametersType &target)
{
  unsigned int i, j;
  int index = 0;
  for (i = 0; i < (unsigned int )source.size(); i++)
    {
    for (j = 0; j < m_MeasurementVectorSize; j++)
      {
      target[index] = source[i][j];
      ++index;
      }
    }
}

template< class TKdTree >
void
KdTreeBasedKmeansEstimator< TKdTree >
::CopyParameters(InternalParametersType &source, InternalParametersType &target)
{
  unsigned int i, j;
  for (i = 0; i < (unsigned int)source.size(); i++)
    {
    for (j = 0; j < m_MeasurementVectorSize; j++)
      {
      target[i][j] = source[i][j];
      }
    }
}

template< class TKdTree >
void
KdTreeBasedKmeansEstimator< TKdTree >
::StartOptimization()
{
  unsigned int i;
  MeasurementVectorType lowerBound;
  MeasurementVectorType upperBound; 
  MeasurementVectorTraits::SetLength( lowerBound, this->m_MeasurementVectorSize );
  MeasurementVectorTraits::SetLength( upperBound, this->m_MeasurementVectorSize );

  FindSampleBound<SampleType>(m_KdTree->GetSample(),
                              m_KdTree->GetSample()->Begin(), 
                              m_KdTree->GetSample()->End(),
                              lowerBound,
                              upperBound);

  InternalParametersType previousPosition;
  //previousPosition.resize(m_Parameters.size() / m_MeasurementVectorSize);
  InternalParametersType currentPosition;
  //currentPosition.resize(m_Parameters.size() / m_MeasurementVectorSize);

  
  for( i=0; i< m_Parameters.size()/ m_MeasurementVectorSize; i++ )
    {
    ParameterType m;
    ParameterType m1;
    MeasurementVectorTraits::SetLength(m, m_MeasurementVectorSize );
    MeasurementVectorTraits::SetLength(m1, m_MeasurementVectorSize );
    previousPosition.push_back( m );
    currentPosition.push_back( m1 );
    }

  
  this->CopyParameters(m_Parameters, currentPosition);
  m_CurrentIteration = 0;
  std::vector< int > validIndexes;

  for (i = 0; i < (unsigned int)(m_Parameters.size() / m_MeasurementVectorSize); i++)
    {
    validIndexes.push_back(i);
    }

  m_GenerateClusterLabels = false;

  while(true)
    {
    this->CopyParameters(currentPosition, previousPosition);
    m_CandidateVector.SetCentroids(currentPosition);
    this->Filter(m_KdTree->GetRoot(), validIndexes,
                 lowerBound, upperBound);
    m_CandidateVector.UpdateCentroids();
    m_CandidateVector.GetCentroids(currentPosition);
     
    if(m_CurrentIteration >= m_MaximumIteration) 
      {
      break;
      } 

    m_CentroidPositionChanges = 
      this->GetSumOfSquaredPositionChanges(previousPosition, 
                                           currentPosition);
    if (m_CentroidPositionChanges <= m_CentroidPositionChangesThreshold)
      {
      break;
      }

    m_CurrentIteration++;
    }

  if ( m_UseClusterLabels )
    {
    m_GenerateClusterLabels = true;
    m_ClusterLabels.clear();
    m_ClusterLabels.resize(m_KdTree->GetSample()->Size());
    for (i = 0; i < (unsigned int)(m_Parameters.size() / m_MeasurementVectorSize); i++)
      {
      validIndexes.push_back(i);
      }

    this->Filter(m_KdTree->GetRoot(), validIndexes,
                 lowerBound, upperBound);
    }

  this->CopyParameters(currentPosition, m_Parameters);
}

} // end of namespace Statistics
} // end namespace itk

#endif
