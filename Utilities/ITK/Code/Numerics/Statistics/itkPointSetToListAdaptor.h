/*=========================================================================

Program:   Insight Segmentation & Registration Toolkit
Module:    $RCSfile: itkPointSetToListAdaptor.h,v $
Language:  C++
Date:      $Date: 2005/09/30 17:40:35 $
Version:   $Revision: 1.15 $

Copyright (c) Insight Software Consortium. All rights reserved.
See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

This software is distributed WITHOUT ANY WARRANTY; without even 
the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkPointSetToListAdaptor_h
#define __itkPointSetToListAdaptor_h

#include <typeinfo>

#include "itkPointSet.h"
#include "itkListSampleBase.h"
#include "itkSmartPointer.h"

namespace itk{ 
namespace Statistics{

/** \class PointSetToListAdaptor
 *  \brief This class provides ListSampleBase interfaces to ITK PointSet
 *
 * After calling SetPointSet(PointSet*) method to plug-in 
 * the PointSet object, users can use Sample interfaces to access 
 * PointSet data. This adaptor assumes that the PointsContainer is
 * actual storage for measurment vectors. In other words, PointSet's
 * points are equal to measurement vectors. This class totally ignores
 * PointsDataContainer.
 * 
 * \sa Sample, ListSampleBase, DefaultStaticMeshTraits, PointSet
 */

template < class TPointSet >
class ITK_EXPORT PointSetToListAdaptor :
    public ListSampleBase< typename TPointSet::PointType >
{
public:
  /** Standard class typedefs */
  typedef PointSetToListAdaptor Self;
  typedef ListSampleBase< typename TPointSet::PointType > Superclass ;
  typedef SmartPointer< Self > Pointer;
  typedef SmartPointer<const Self> ConstPointer;
  
  /** Run-time type information (and related methods). */
  itkTypeMacro(PointSetToListAdaptor, ListSampleBase) ;
  
  /** Method for creation through the object factory. */
  itkNewMacro(Self) ;
  
  /** the number of components in a measurement vector */
  itkStaticConstMacro(MeasurementVectorSize, unsigned int,
                      TPointSet::PointDimension);

  /** PointSet typedefs */
  typedef TPointSet PointSetType;
  typedef typename TPointSet::Pointer PointSetPointer ;
  typedef typename TPointSet::PointIdentifier InstanceIdentifier;
  typedef typename TPointSet::PointsContainerPointer PointsContainerPointer ;
  typedef typename TPointSet::PointsContainerIterator PointsContainerIterator ;
  typedef typename TPointSet::PointType PointType ;


  /** Superclass typedefs for Measurement vector, measurement, 
   * Instance Identifier, frequency, size, size element value */
  typedef typename Superclass::MeasurementType MeasurementType ;
  typedef typename Superclass::MeasurementVectorType MeasurementVectorType;
  typedef MeasurementVectorType ValueType ;
  typedef typename Superclass::FrequencyType FrequencyType ;
  typedef typename Superclass::TotalFrequencyType TotalFrequencyType ;
  typedef typename Superclass::MeasurementVectorSizeType MeasurementVectorSizeType;

  /** Method to set the point set */
  void SetPointSet(TPointSet* pointSet) ;

  /** Method to get the point set */
  TPointSet* GetPointSet() ;

  /** returns the number of measurement vectors in this container*/
  unsigned int Size() const ;

  /** returns the measurement vector that is specified by the instance
   * identifier argument. */
  const MeasurementVectorType & GetMeasurementVector(const InstanceIdentifier &id) const;

  /** sets the "dim" dimensional component value of the measurement vector
   * that is specified by "id". */
  void SetMeasurement(const InstanceIdentifier &id, 
                      const unsigned int &dim,
                      const MeasurementType &value) ;

  /** returns 1 as other subclasses of ListSampleBase does */
  FrequencyType GetFrequency(const InstanceIdentifier &id) const ;

  /** returns the size of this container */
  TotalFrequencyType GetTotalFrequency() const ;

  void SetMeasurementVectorSize( const MeasurementVectorSizeType s )
    {
    // MV size of the point set is fixed as that of the point's dimension
    // Throw exception if user tries to set it to a different value
    if( s != MeasurementVectorSize )
      {
      itkExceptionMacro( << "PointSetToListAdaptor's measurement vector lengths"
          << " cannot be set to " << s );
      }
    }

  MeasurementVectorSizeType GetMeasurementVectorSize() const
    {
    return MeasurementVectorSize;
    }
    

 
  class Iterator
  {
  public:
    
    Iterator(){}
    
    Iterator(PointsContainerIterator iter)
      :m_Iter(iter)
    {}
    
    FrequencyType GetFrequency() const
    { return 1 ;}

    const MeasurementVectorType & GetMeasurementVector() const
    { return (MeasurementVectorType&) m_Iter.Value() ;} 

    InstanceIdentifier GetInstanceIdentifier() const
    { return m_Iter.Index() ;}

    Iterator& operator++()
    { ++m_Iter ; return *this ;}
    
    Iterator& operator--()
    { --m_Iter ; return *this ;}

    bool operator!=(const Iterator &it)
    { return (m_Iter != it.m_Iter) ;}
    
    bool operator==(const Iterator &it)
    { return (m_Iter == it.m_Iter) ;}
    
    Iterator& operator = (const Iterator &iter)
    { 
      m_Iter = iter.m_Iter; 
      return iter ;
    }

    Iterator(const Iterator &iter)
    { m_Iter = iter.m_Iter; }
    
  private:
    PointsContainerIterator m_Iter ;
  } ;
   
 
  class ConstIterator
  {
  public:
    
    ConstIterator(){}
    
    ConstIterator(PointsContainerIterator iter)
      :m_Iter(iter)
    {}
    
    FrequencyType GetFrequency() const
    { return 1 ;}

    const MeasurementVectorType & GetMeasurementVector() const
    { return (MeasurementVectorType&) m_Iter.Value() ;} 

    InstanceIdentifier GetInstanceIdentifier() const
    { return m_Iter.Index() ;}

    ConstIterator& operator++()
    { ++m_Iter ; return *this ;}
    
    ConstIterator& operator--()
    { --m_Iter ; return *this ;}

    bool operator!=(const ConstIterator &it)
    { return (m_Iter != it.m_Iter) ;}
    
    bool operator==(const ConstIterator &it)
    { return (m_Iter == it.m_Iter) ;}
    
    ConstIterator& operator = (const ConstIterator &iter)
    { 
      m_Iter = iter.m_Iter; 
      return iter ;
    }

    ConstIterator(const ConstIterator &iter)
    { m_Iter = iter.m_Iter; }
    
  private:
    PointsContainerIterator m_Iter ;
  } ;
   
  /** returns an iterator that points to the beginning of the container */
  Iterator Begin()
  { 
    Iterator iter(m_PointsContainer->Begin());
    return iter; 
  }
  
  /** returns an iterator that points to the end of the container */
  Iterator End()        
  {
    Iterator iter(m_PointsContainer->End()); 
    return iter; 
  }
   
  /** returns a const iterator that points to the beginning of the container */
  ConstIterator Begin() const
  { 
    ConstIterator iter(m_PointsContainer->Begin());
    return iter; 
  }
  
  /** returns an iterator that points to the end of the container */
  ConstIterator End() const
  {
    ConstIterator iter(m_PointsContainer->End()); 
    return iter; 
  }
  
protected:
  PointSetToListAdaptor(); 
  
  virtual ~PointSetToListAdaptor() {}
  void PrintSelf(std::ostream& os, Indent indent) const;  

private:
  PointSetToListAdaptor(const Self&) ; //purposely not implemented
  void operator=(const Self&) ; //purposely not implemented

  /** the PointSet data source pointer */
  mutable PointSetPointer m_PointSet ;
  /** the points container which will be actually used for storing
   * measurement vectors */
  PointsContainerPointer m_PointsContainer ;
  /** temporary points for conversions */
  mutable PointType m_TempPoint ;
} ; // end of class PointSetToListAdaptor

} // end of namespace Statistics
} // end of namespace itk


#ifndef ITK_MANUAL_INSTANTIATION
#include "itkPointSetToListAdaptor.txx"
#endif

#endif
