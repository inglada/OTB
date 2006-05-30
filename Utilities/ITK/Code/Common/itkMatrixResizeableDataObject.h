/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkMatrixResizeableDataObject.h,v $
  Language:  C++
  Date:      $Date: 2004/03/03 19:12:29 $
  Version:   $Revision: 1.3 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkMatrixResizeableDataObject_h
#define __itkMatrixResizeableDataObject_h

#include "itkDataObject.h"
#include "vnl/vnl_matrix.h"

namespace itk
{
       
/**
 * \class MatrixResizeableDataObject
 * \brief Allows for a vnl matrix to be a data
 * object with the flexibility of being resizable.
 *
 * \ingroup DataProcessing  
 **/

template<typename TItemType>
class MatrixResizeableDataObject : public DataObject, public vnl_matrix<TItemType> {
public:

  /** Standard class typedefs. */
  typedef MatrixResizeableDataObject Self;
  typedef DataObject Superclass;
  typedef SmartPointer<Self>   Pointer;
  typedef SmartPointer<const Self>  ConstPointer;

  /** Method for creation through the object factory. */
  itkNewMacro(Self);

  /** Run-time type information (and related methods). */
  itkTypeMacro(MatrixResizeableDataObject, DataObject);

protected:

    /** Default Constructor. */
  MatrixResizeableDataObject();

  /** Default Destructor. */
  ~MatrixResizeableDataObject();
};

} // end namespace itk

#ifndef ITK_MANUAL_INSTANTIATION
#include "itkMatrixResizeableDataObject.txx"
#endif

#endif
