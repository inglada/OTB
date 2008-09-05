/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkExhaustiveOptimizer.h,v $
  Language:  C++
  Date:      $Date: 2008-05-06 09:42:44 $
  Version:   $Revision: 1.5 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkExhaustiveOptimizer_h
#define __itkExhaustiveOptimizer_h

#include "itkSingleValuedNonLinearOptimizer.h"

namespace itk
{
  
/** \class ExhaustiveOptimizer
 * \brief Optimizer that fully samples a grid on the parametric space.
 *
 * This optimizer is equivalent to an exahaustive search in a discrete grid
 * defined over the parametric space. The grid is centered on the initial
 * position. The subdivisions of the grid along each one of the dimensions
 * of the parametric space is defined by an array of number of steps. 
 *
 * A typical use is to plot the metric space to get an idea of how noisy it
 * is. An example is given below, where it is desired to plot the metric
 * space with respect to translations along x, y and z in a 3D registration
 * application:
 *     Here it is assumed that the transform is Euler3DTransform.
 *
 * \code
 * 
 * OptimizerType::StepsType steps( m_Transform->GetNumberOfParameters() );
 * steps[1] = 10;
 * steps[2] = 10;
 * steps[3] = 10;
 * m_Optimizer->SetNumberOfSteps( steps );
 * m_Optimizer->SetStepLength( 2 );
 *
 * \endcode
 *
 * The optimizer throws IterationEvents after every iteration. We use this to plot 
 * the metric space in an image as follows:
 *
 * \code
 *
 *  if( itk::IterationEvent().CheckEvent(& event ) )
 *    {
 *    IndexType index;
 *    index[0] = m_Optimizer->GetCurrentIndex()[0];
 *    index[1] = m_Optimizer->GetCurrentIndex()[1];
 *    index[2] = m_Optimizer->GetCurrentIndex()[2];
 *    image->SetPixel( index, m_Optimizer->GetCurrentValue() );
 *    }
 * 
 * \endcode
 *
 * The image size is expected to be 11 x 11 x 11.
 *
 * If you wish to use different step lengths along each parametric axis,
 * you can use the SetScales() method. This accepts an array, each element
 * represents the number of subdivisions per step length. For instance scales
 * of [0.5 1 4] along with a step length of 2 will cause the optimizer
 * to search the metric space on a grid with x,y,z spacing of [1 2 8].
 *
 * Physical dimensions of the grid are influenced by both the scales and
 * the number of steps along each dimension, a side of the region is
 * stepLength*(2*numberOfSteps[d]+1)*scaling[d].
 *
 * \ingroup Numerics Optimizers
 */
class ITK_EXPORT ExhaustiveOptimizer : 
                    public SingleValuedNonLinearOptimizer
{
public:
  /** Standard "Self" typedef. */
  typedef ExhaustiveOptimizer            Self;
  typedef SingleValuedNonLinearOptimizer Superclass;
  typedef SmartPointer<Self>             Pointer;
  typedef SmartPointer<const Self>       ConstPointer;
  
  typedef Array< unsigned long > StepsType;
  /** Method for creation through the object factory. */
  itkNewMacro(Self);
  
  /** Run-time type information (and related methods). */
  itkTypeMacro( ExhaustiveOptimizer,           SingleValuedNonLinearOptimizer   );
 
  virtual void    StartOptimization( void );

  void StartWalking( void );
  void ResumeWalking( void );
  void StopWalking(void);
  
  itkSetMacro( StepLength, double );
  itkSetMacro( NumberOfSteps, StepsType );
  itkGetConstReferenceMacro( StepLength, double );
  itkGetConstReferenceMacro( NumberOfSteps, StepsType );
  itkGetConstReferenceMacro( CurrentValue, MeasureType );
  itkGetConstReferenceMacro( MaximumMetricValue, MeasureType );
  itkGetConstReferenceMacro( MinimumMetricValue, MeasureType );
  itkGetConstReferenceMacro( MinimumMetricValuePosition, ParametersType );
  itkGetConstReferenceMacro( MaximumMetricValuePosition, ParametersType );
  itkGetConstReferenceMacro( CurrentIndex, ParametersType );
  itkGetConstReferenceMacro( MaximumNumberOfIterations, unsigned long );


protected:
  ExhaustiveOptimizer();
  virtual ~ExhaustiveOptimizer() {};
  void PrintSelf(std::ostream& os, Indent indent) const;

  /** Advance to the next grid position. */
  void AdvanceOneStep( void );
  void IncrementIndex( ParametersType & param );

  
private:  
  ExhaustiveOptimizer(const Self&); //purposely not implemented
  void operator=(const Self&);//purposely not implemented

protected:
  MeasureType          m_CurrentValue;
  StepsType            m_NumberOfSteps;
  unsigned long        m_CurrentIteration;
  bool                 m_Stop;
  unsigned int         m_CurrentParameter;
  double               m_StepLength; 
  ParametersType       m_CurrentIndex;
  unsigned long        m_MaximumNumberOfIterations;
  MeasureType          m_MaximumMetricValue;
  MeasureType          m_MinimumMetricValue;
  ParametersType       m_MinimumMetricValuePosition;
  ParametersType       m_MaximumMetricValuePosition;
  
};

} // end namespace itk

#endif
