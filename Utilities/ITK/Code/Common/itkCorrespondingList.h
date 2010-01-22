/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkCorrespondingList.h,v $
  Language:  C++
  Date:      $Date: 2003-09-10 14:29:05 $
  Version:   $Revision: 1.2 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkCorrespondingList_h
#define __itkCorrespondingList_h

#include <list>
#include "itkCorrespondingList.h"

namespace itk
{

/**
 * \class CorrespondingList
 * \brief Part of the itkCorrespondenceDataStructure.
 *
 *
 * 
 * */

template <typename TItemType, int VCliqueSize>
class CorrespondingList : public std::list< TItemType >
{
public:

  /** Get the number of items stored in the blox. */
  unsigned long int GetSize()
    {return this->size(); }

  /** Set and Get functions for thesecondary indices in the base node clique. */
  int GetIndex(int i){return m_Index[i];}
  void SetIndex(int i, int value){m_Index[i] = value;}

  /** Constructor */
  CorrespondingList();

  /** Destructor */
  ~CorrespondingList();

private:
  
  /** Array to hold secondary indices of the base node clique */
  int m_Index[VCliqueSize];
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkCorrespondingList.txx"
#endif

#endif
