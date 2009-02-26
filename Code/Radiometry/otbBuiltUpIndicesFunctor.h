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
#ifndef __otbBuiltUpIndicesFunctor_h
#define __otbBuiltUpIndicesFunctor_h

#include "otbMath.h"
#include "itkVariableLengthVector.h"
#include "otbVegetationIndicesFunctor.h"

namespace otb
{
namespace Functor
{
/**
   * \class TM4AndTM5IndexBase
   * \brief Base class for Green And Red channels of Spot Images
   *
   *  Implement operators for UnaryFunctorImageFilter templated with a
   *  VectorImage and BinaryFunctorImageFilter templated with single
   *  images.
   *  Subclasses should NOT overload operators, they must  re-implement
   *  the Evaluate() method.
   *
   * \ingroup Radiometry
 */
template<class TInput1, class TInput2, class TOutput>
class TM4AndTM5IndexBase
{
public:
  /// Vector pixel type used to support both vector images and multiple
  /// input images
  typedef itk::VariableLengthVector<TInput1> InputVectorType;

  // Operator on vector pixel type
  inline TOutput operator()(const InputVectorType & inputVector)
  {
    return this->Evaluate(inputVector[m_TM4Index-1],static_cast<TInput2>(inputVector[m_TM5Index-1]));
  }

  // Binary operator
  inline TOutput operator()(const TInput1 &tm4, const TInput2 &tm5)
  {
    return this->Evaluate(tm4,tm5);
  };
  /// Constructor
  TM4AndTM5IndexBase() : m_TM4Index(4), m_TM5Index(5) {};
  /// Desctructor
  virtual ~TM4AndTM5IndexBase() {};

  /// Set TM4 Index
  void SetIndex1(unsigned int channel)
  {
    m_TM4Index = channel;
  }
  /// Get TM4 Index
  unsigned int GetIndex1()
  {
    return m_TM4Index;
  }
  /// Set TM5 Index
  void SetIndex2(unsigned int channel)
  {
    m_TM5Index = channel;
  }
  /// Get TM5 Index
  unsigned int GetIndex2()
  {
    return m_TM5Index;
  }
protected:
  // This method must be reimplemented in subclasses to actually
  // compute the index value
  virtual TOutput Evaluate(const TInput1 & tm4, const TInput2 & tm5) const = 0;

private:
  unsigned int m_TM4Index;
  unsigned int m_TM5Index;
};





/** \class NDBI
 *  \brief This functor computes the Normalized Difference Built Up Index (NDBI)
 *
 *  [ ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class NDBI : public TM4AndTM5IndexBase<TInput1,TInput2,TOutput>
{
public:
  /// Constructor
  NDBI() {};
  /// Desctructor
  ~NDBI() {};
  // Operator on r and nir single pixel values
protected:
  inline TOutput Evaluate(const TInput1 &pTM4, const TInput2 &pTM5) const
  {
    double dTM4 = static_cast<double>(pTM4);
    double dTM5 = static_cast<double>(pTM5);
    if ( dTM5+dTM4 == 0 )
      {
      return static_cast<TOutput>(0.);
      }

    return ( static_cast<TOutput>((dTM5-dTM4)/(dTM5+dTM4)) );
  }
};

/** \class ISU
 *  \brief This functor computes the Index surfaces built (ISU)
 *
 *  [ ]
 *
 *  \ingroup Functor
 * \ingroup Radiometry
 */
template <class TInput1, class TInput2, class TOutput>
class ISU : public RAndNIRIndexBase<TInput1,TInput2,TOutput>
{
public:
  /// Constructor
  ISU() : m_A(100.), m_B(25.) {};
  /// Desctructor
  ~ISU() {};

  /** Set/Get A correction */
  void SetA(const double pA)
  {
    m_A = pA;
  }
  double GetA(void)const
  {
    return ( m_A );
  }
  /** Set/Get B correction */
  void SetB(const double pB)
  {
    m_B = pB;
  }
  double GetB(void)const
  {
    return ( m_B );
  }

protected:
  inline TOutput Evaluate(const TInput1 &pRed, const TInput2 &pNIR) const
  {
    double dRed = static_cast<double>(pRed);
    double dNIR = static_cast<double>(pNIR);
    if ( dNIR == 0 )
      {
      return static_cast<TOutput>(0.);
      }

    return ( static_cast<TOutput>(m_A - (m_B*dRed)/(dNIR)) );
  }

private:
  double m_A;
  double m_B;
};


} // namespace Functor
} // namespace otb

#endif
