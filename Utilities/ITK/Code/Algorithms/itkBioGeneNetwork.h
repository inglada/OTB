/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkBioGeneNetwork.h,v $
  Language:  C++
  Date:      $Date: 2004-10-02 21:08:24 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkBioGeneNetwork_h_
#define __itkBioGeneNetwork_h_

#ifdef _MSC_VER
#pragma warning ( disable : 4786 )
#endif

#include <vector>

namespace itk {

namespace bio {

/** \class GeneNetwork
 * \brief This class implement the abstraction of a biological gene network.
 * 
 * This class is implemented to iteratively compute the state of the gene network
 * following update rules (typical Partial Differential Equations).
 * 
 */
class GeneNetwork  
{
public:
  typedef   std::vector< float >       ProteomeType;
  typedef   std::vector< float >       DomainsType;

public:
  GeneNetwork();
  virtual ~GeneNetwork();

  void Copy( const GeneNetwork & genome );


private:

  // This array contains the concentrations each protein
  // in the cell at a certain time.
  ProteomeType              m_ProteinConcentration;

  // This array contains the concentrations domain affinities 
  // in the cell at a certain time. Since each protein may 
  // have multiple domains and each domain have affinities
  // for standard domains in the regulatory section of genes,
  // this array is indexed by the domains in the regulatory
  // section. The final value on each entry indicates the 
  // probability of a domain to be bound during a certain time
  // interval.
  DomainsType               m_DomainConcentration;

};


} // end namespace bio

} // end namespace itk

#endif


