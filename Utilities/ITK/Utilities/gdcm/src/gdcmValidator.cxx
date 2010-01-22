/*=========================================================================
                                                                                
  Program:   gdcm
  Module:    $RCSfile: gdcmValidator.cxx,v $
  Language:  C++
  Date:      $Date: 2006-02-20 15:56:58 $
  Version:   $Revision: 1.5 $
                                                                                
  Copyright (c) CREATIS (Centre de Recherche et d'Applications en Traitement de
  l'Image). All rights reserved. See Doc/License.txt or
  http://www.creatis.insa-lyon.fr/Public/Gdcm/License.html for details.
                                                                                
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notices for more information.
                                                                                
=========================================================================*/

#include "gdcmValidator.h"
#include "gdcmElementSet.h"
#include "gdcmBinEntry.h"
#include "gdcmValEntry.h"
#include "gdcmDebug.h" // hidden way to include sstream

namespace gdcm 
{

Validator::Validator()
{
}

Validator::~Validator()
{
}

// Function to compare the VM found while parsing d->GetValue()
// compare to the one from the dictionary
bool CheckVM(ValEntry *v)
{
  const std::string &s = v->GetValue();
  std::string::size_type n = s.find("\\");
  if ( n == s.npos ) // none found
  {
    n = 0;
  }
  n++; // number of '\' + 1 == Value Multiplicity

  unsigned int m;
  itksys_ios::istringstream is;
  is.str( v->GetVM());
  is >> m;

  return n == m;
}

void Validator::SetInput(ElementSet *input)
{
  // berk for now SetInput do two things at the same time
  gdcm::DocEntry *d=input->GetFirstEntry();
  while(d)
  {
    if ( gdcm::BinEntry *b = dynamic_cast<gdcm::BinEntry*>(d) )
    {
//      copyH->InsertBinEntry( b->GetBinArea(),b->GetLength(),
//        b->GetGroup(),b->GetElement(),
//        b->GetVR() );
        (void)b;
    }
    else if ( gdcm::ValEntry *v = dynamic_cast<gdcm::ValEntry*>(d) )
    {   
      if ( !CheckVM(v) )
      {
        std::cout << "This DICOM contains one wrong tag:" << 
        v->GetValue() << " " <<
        v->GetGroup() << "," << v->GetElement() << "," <<
        v->GetVR() << " " << v->GetVM() << " " << v->GetName() << std::endl;
      }
    }
    else
    {
      // We skip pb of SQ recursive exploration
    }
    d=input->GetNextEntry();
  }
}

} // end namespace gdcm
