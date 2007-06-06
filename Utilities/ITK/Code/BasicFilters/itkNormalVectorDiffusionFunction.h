/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNormalVectorDiffusionFunction.h,v $
  Language:  C++
  Date:      $Date: 2003/09/10 14:28:53 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

     =========================================================================*/
#ifndef __itkNormalVectorDiffusionFunction_h_
#define __itkNormalVectorDiffusionFunction_h_

#include "itkNormalVectorFunctionBase.h"
#include "itkNumericTraits.h"
#include <math.h>

namespace itk {

/**
 * \class NormalVectorDiffusionFunction
 *
 * \brief This class defines all the necessary functionality for performing
 * isotropic and anisotropic diffusion operations on vector neighborhoods from
 * a sparse image.
 *
 * \par
 * This class implements the actual computations for performing isotropic and
 * anisotropic diffusion operations on a neighborhood of unit length
 * vectors. Moreover, this processing is intrinsic to a manifold as specified
 * by the ManifoldNormal member variables in the nodes of the sparse image.
 *
 * \par
 * Since the only difference between isotropic and anisotropic diffusion is the
 * exectution of 1 extra line of code, we have implemented both in this class
 * and made the choice between the two depend on a parameter (see below).
 
 * \par PARAMETERS
 * The choice between is isotropic/anisotropic diffusion is made by the
 * parameter NormalProcessType. A value of 0 corresponds to isotropic diffusion
 * whereas a value of 1 corresponds to anisotropic diffusion. If anisotropic
 * diffusion is chosen, the parameter ConductanceParameter should be set. This
 * conductance parameter determines the level of feature preservation.
 *
 * \par IMPORTANT
 * This class works on SparseImage neighborhoods. Before using this class
 * please read the documentation for SparseImage. Also the documentation for 
 * ImplicitManifoldNormalVectorField class will be helpful in understanding how
 * to use this class as a function object.
 */
template <class TSparseImageType>
class ITK_EXPORT NormalVectorDiffusionFunction 
  :public NormalVectorFunctionBase <TSparseImageType>
{
public:
  /** Standard class typedef. */
  typedef NormalVectorDiffusionFunction Self;
  typedef NormalVectorFunctionBase <TSparseImageType> Superclass;
  typedef SmartPointer<Self> Pointer;
  typedef SmartPointer<const Self> ConstPointer;

  /** Run-time type information (and related methods) */
  itkTypeMacro( NormalVectorDiffusionFunction, NormalVectorFunctionBase);
   
  /** Image dimension derived from the superclass. */
  itkStaticConstMacro(ImageDimension, unsigned int, Superclass::ImageDimension);

  /** Standard New macro. */
  itkNewMacro(Self);

  /** Typedefs from the superclass. */
  typedef typename Superclass::TimeStepType     TimeStepType;
  typedef typename Superclass::RadiusType       RadiusType;
  typedef typename Superclass::NeighborhoodType NeighborhoodType;
  typedef typename Superclass::FloatOffsetType  FloatOffsetType;
  typedef typename Superclass::IndexType        IndexType;
  typedef typename Superclass::SparseImageType  SparseImageType;
  typedef typename Superclass::NodeType         NodeType;
  typedef typename Superclass::NodeValueType    NodeValueType;
  typedef typename Superclass::NormalVectorType NormalVectorType;

  /** This method is used to choose between isotropic/anisotropic filtering. A
      parameter value of 0 indicates isotropic diffusion and is the
      default. Parameter value 1 is anisotropic diffusion. When using
      anisotropic diffusion the conductance parameter should also be set. */
  void SetNormalProcessType( int npt )
  { m_NormalProcessType = npt; }

  /** This method returns the isotropic/anisotropic filtering parameter. */
  int GetNormalProcessType() const 
  { return m_NormalProcessType; }

  /** This method sets the conductance parameter used in anisotropic
   * filtering. Useful values for processing 2D and 3D shapes are between
   *  0.1 and 0.25. Lower values preserve more shape features, higher values
   *  smooth more. As the conductance parameter large, the processing becomes
   *  isotropic. Default is 0. */
  void SetConductanceParameter( NodeValueType cp )
  {
    m_ConductanceParameter = cp + static_cast<NodeValueType> (0.001);
    // we add a minimum conductance to avoid divide by zero
    // can make this a parameter.
    m_FluxStopConstant = static_cast<NodeValueType>
      (-1.0/(m_ConductanceParameter*m_ConductanceParameter));
  }

  /** This method returns the conductance parameter. */
  NodeValueType GetConductanceParameter() const
  { return m_ConductanceParameter; }

  /** This method returns the internal variable FluxStopConstant. */
  NodeValueType GetFluxStopConstant() const
  { return m_FluxStopConstant; }
  
  /** This function is called from LevelSetNormalImageFilter for all of the
   *  nodes to compute and store the flux vectors (first derivatives of the
   *  normal vectors. ComputeUpdateNormal then takes derivatives of the flux
   *  vectors. This way we avoid repeating the same flux computations. */
  virtual void PrecomputeSparseUpdate( NeighborhoodType &it ) const;

  /** The actual update rule for the normal vectors. */
  virtual NormalVectorType ComputeSparseUpdate (NeighborhoodType &neighborhood,
                                                void *globalData,
                                                const FloatOffsetType &offset) const;
  
private:
  /** The conductance parameter used for anisotropic diffusion. */
  NodeValueType m_ConductanceParameter;

  /** The internal variable used in the FluxStopFunction. It is computed from
   * ConductanceParameter. */
  NodeValueType m_FluxStopConstant;

protected:
  NormalVectorDiffusionFunction();
  ~NormalVectorDiffusionFunction() {}
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** The method called in anisotropic diffusion to inhibit diffusion across
      areas with large curvature. */
  NodeValueType FluxStopFunction( const NodeValueType v ) const
  {
    // the slow ::exp function could be replaced with a lookup table
    if (v<=0.0) return NumericTraits<NodeValueType>::One;
    else return static_cast<NodeValueType>(::exp(m_FluxStopConstant*v));
  }
  
private:
  /** The isotropic/anisotropic filtering choice parameter. */
  int m_NormalProcessType; 

  NormalVectorDiffusionFunction(const Self&); //purposely not implemented
  void operator=(const Self&);                //purposely not implemented
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkNormalVectorDiffusionFunction.txx"
#endif

#endif
