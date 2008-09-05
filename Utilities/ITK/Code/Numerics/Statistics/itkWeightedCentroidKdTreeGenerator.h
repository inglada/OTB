/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkWeightedCentroidKdTreeGenerator.h,v $
  Language:  C++
  Date:      $Date: 2008-04-25 22:36:09 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkWeightedCentroidKdTreeGenerator_h
#define __itkWeightedCentroidKdTreeGenerator_h

#include <vector>

#include "itkSample.h"
#include "itkSubsample.h"
#include "itkKdTree.h"
#include "itkKdTreeGenerator.h"
#include "itkStatisticsAlgorithm.h"

namespace itk{ 
namespace Statistics{

/** \class WeightedCentroidKdTreeGenerator 
 *  \brief This class generates a KdTree object with centroid information.
 * 
 * The KdTree object stores measurment vectors in a k-d tree structure
 * that is a binary tree. The partition value is the median value of one
 * of the k dimension (partition dimension). The partition dimension is
 * determined by the spread of measurement values in each dimension. The
 * partition dimension is the dimension has the widest spread. Our
 * implementation of k-d tree doesn't have any construction or insertion
 * logic. Users should use this class or the KdTreeGenerator class.
 *
 * This class is derived from the KdTreeGenerator class. The only
 * difference between this class and the KdTreeGenerator class is that
 * the nonterminal node type of this class is
 * KdTreeWeightedCentroidNonterminalNode and that of the
 * KdTreeGenerator is KdTreeNonterminalNode. Therefore, the public
 * interface is identical to each other. The nonterminal node generation
 * routines differ.
 *
 * To run this generator, users should provides the bucket size
 * (SetBucketSize method) and the input sample (SetSample method). The
 * Update method will run this generator. To get the resulting KdTree
 * object, call the GetOutput method.
 * 
 * <b>Recent API changes:</b>
 * The static const macro to get the length of a measurement vector,
 * 'MeasurementVectorSize'  has been removed to allow the length of a measurement
 * vector to be specified at run time. It is now obtained from the sample set
 * as input. You may query this length using the function GetMeasurementVectorSize().
 * 
 * \sa KdTree, KdTreeNode, KdTreeWeightedCentroidNonterminalNode, 
 * KdTreeTerminalNode, KdTreeGenerator
 */

template < class TSample >
class ITK_EXPORT WeightedCentroidKdTreeGenerator : 
    public KdTreeGenerator< TSample >
{
public:
  /** Standard class typedefs */
  typedef WeightedCentroidKdTreeGenerator Self ;
  typedef KdTreeGenerator< TSample > Superclass ;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods) */
  itkTypeMacro(WeightedCentroidKdTreeGenerator, KdTreeGenerator);

  /** Method for creation through the object factory. */
  itkNewMacro(Self) ;

  /** typedef alias for the source data container */ 
  typedef typename Superclass::MeasurementVectorType MeasurementVectorType ;
  typedef typename Superclass::MeasurementType MeasurementType ;
  typedef typename Superclass::SubsampleType SubsampleType ;
  typedef typename Superclass::SubsamplePointer SubsamplePointer ;
  typedef typename Superclass::KdTreeType KdTreeType ;
  typedef typename Superclass::KdTreeNodeType KdTreeNodeType ;


protected:
  /** Constructor */
  WeightedCentroidKdTreeGenerator() ;

  /** Destructor */
  virtual ~WeightedCentroidKdTreeGenerator() {}

  void PrintSelf(std::ostream& os, Indent indent) const ;

  /** Nonterminal node generation routine */
  virtual KdTreeNodeType* GenerateNonterminalNode(unsigned int beginIndex,
                                                  unsigned int endIndex,
                                                  MeasurementVectorType 
                                                  &lowerBound,
                                                  MeasurementVectorType 
                                                  &upperBound,
                                                  unsigned int level) ;

private:
  WeightedCentroidKdTreeGenerator(const Self&) ; //purposely not implemented
  void operator=(const Self&) ; //purposely not implemented

  MeasurementVectorType m_TempLowerBound ;
  MeasurementVectorType m_TempUpperBound ;
  MeasurementVectorType m_TempMean ;
} ; // end of class

} // end of namespace Statistics 
} // end of namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkWeightedCentroidKdTreeGenerator.txx"
#endif

#endif




