/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkResourceProbesCollectorBase.txx,v $
  Language:  C++
  Date:      $Date: 2008-11-04 23:45:31 $
  Version:   $Revision: 1.2.2.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkResourceProbesCollectorBase_txx
#define __itkResourceProbesCollectorBase_txx

#include "itkResourceProbesCollectorBase.h"
#include <iostream>

namespace itk
{

template <class TProbe>
ResourceProbesCollectorBase<TProbe>
::~ResourceProbesCollectorBase()
{
}

template <class TProbe>
void 
ResourceProbesCollectorBase<TProbe>
::Start(const char * id)
{
  // if the probe does not exist yet, it is created.
  this->m_Probes[ id ].Start(); 
}

template <class TProbe>
void 
ResourceProbesCollectorBase<TProbe>
::Stop(const char * id)
{
  IdType tid = id;
  typename MapType::iterator  pos = this->m_Probes.find( tid );
  if ( pos == this->m_Probes.end() )
    {
    itkGenericExceptionMacro( << "The probe \""<< id<< "\" does not exist. It can not be stopped." );
    return;
    }
  pos->second.Stop();
}


template <class TProbe>
void 
ResourceProbesCollectorBase<TProbe>
::Report( std::ostream & os ) const
{
  typename MapType::const_iterator probe = this->m_Probes.begin();
  typename MapType::const_iterator end   = this->m_Probes.end();

  if ( probe == end )
    {
    os << "No probes have been created" << std::endl;
    return;
    }

  os.width(20);
  os <<  " Probe Tag ";
  os.width(10);
  os <<  " Starts ";
  os.width(10);
  os <<  " Stops  ";
  os.width(15);
  os <<  probe->second.GetType() << " (" << probe->second.GetUnit() << ")";
  os << std::endl;
  
  while( probe != end )
    {
    os.width(20);
    os <<  probe->first << "  ";
    os.width(10);
    os <<  probe->second.GetNumberOfStarts() <<  "   ";
    os.width(10);
    os <<  probe->second.GetNumberOfStops() <<  "   ";
    os.width(15);
    os <<  probe->second.GetMean();
    os << std::endl;
    probe++;
    }

}
    
   

template <class TProbe>
void 
ResourceProbesCollectorBase<TProbe>
::Clear(void) 
{
  this->m_Probes.clear();
}


} // end namespace itk

#endif //__itkResourceProbesCollectorBase_txx
