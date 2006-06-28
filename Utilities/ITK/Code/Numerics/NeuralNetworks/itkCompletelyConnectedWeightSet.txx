/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCompletelyConnectedWeightSet.txx,v $
  Language:  C++
  Date:      $Date: 2006/04/17 19:35:36 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

#ifndef __itkCompletelyConnectedWeightSet_txx
#define __itkCompletelyConnectedWeightSet_txx

#include "itkCompletelyConnectedWeightSet.h"

namespace itk
{
namespace Statistics
{

template<class TVector, class TOutput>
CompletelyConnectedWeightSet<TVector,TOutput>
::CompletelyConnectedWeightSet()
{
}

template<class TVector, class TOutput>
void
CompletelyConnectedWeightSet<TVector,TOutput>
::SetCompleteConnectivity()
{
  vnl_matrix<int> c;
  unsigned int rows = WeightSetBase<TVector, TOutput>::GetNumberOfOutputNodes();
  unsigned int cols = WeightSetBase<TVector, TOutput>::GetNumberOfInputNodes();
  std::cout << "Connectivity matrix size= " << rows << " " << cols << std::endl;
  c.set_size(rows, cols);
  c.fill(1);
  WeightSetBase<TVector, TOutput> ::SetConnectivityMatrix(c);
  this->Modified();
}


/** Print the object */
template<class TVector, class TOutput>
void  
CompletelyConnectedWeightSet<TVector,TOutput>
::PrintSelf( std::ostream& os, Indent indent ) const 
{ 
  os << indent << "CompletelyConnectedWeightSet(" << this << ")" << std::endl; 
  Superclass::PrintSelf( os, indent ); 
} 

} // end namespace Statistics
} // end namespace itk

#endif
