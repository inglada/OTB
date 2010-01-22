/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMP.h,v $
  Language:  C++
  Date:      $Date: 2009-01-30 21:53:03 $
  Version:   $Revision: 1.11 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkFEMP_h
#define __itkFEMP_h

namespace itk {
namespace fem {

/**
 * \class FEMP
 * \brief Pointer used to store polymorphic elements in STL arrays.
 *
 * FEMP holds a pointer to objects of class T and its derived classes. it
 * behaves like a special kind of pointer. Special pointers to object can
 * be used to store polymorphic arrays in STL. The basic idea of the special
 * pointer is: whatever you do to the pointer (object of class FEMP), is
 * also reflected on the object within (pointed to by m_Data member). For
 * example: if you copy the special pointer, an object within is also copied.
 *
 * Class T should have a member Clone() which produces a copy of an object.
 * This is important in polymorphic classes, where object of the derived
 * class should be created when copying an existing object.
 *
 * Class T should also include typedefs T::Pointer and T::ConstPointer that
 * define standard pointers to the class. Note that these could be
 * SmartPointer classes.
 */
template<class T>
class FEMP
{
public:

  /**
   * Default constructor makes sure that m_Data is 0,
   * to prevent problems when deleting m_Data object
   * on destruction.
   */
  FEMP() : m_Data(0)
    {
    }
  
  /**
   * Copy constructor. Clone() method is called
   * to duplicate the existing object.
   */
  FEMP(const FEMP& x)
    {
    if (x.m_Data)
      {
      m_Data=static_cast<T*>(&*x.m_Data->Clone());
      }
    else
      {
      m_Data=0;
      }
    }

  /**
   * Conversion constructor from T::Pointer to FEMP<T>.
   * The object T* must exist and we take ownership of object T*.
   * If you want to create a copy of object and take ownership of that,
   * use: FEMP(x->Clone()) instead of FEMP(x).
   */
  explicit FEMP(typename T::Pointer x) : m_Data(x)
    {}
  
  /**
   * Destructor of a special pointer class also destroys the actual object.
   */
  ~FEMP()
    {
    #ifndef FEM_USE_SMART_POINTERS
    delete m_Data;
    #endif
    }

  /**
   * Asignment operator
   */  
  const FEMP& operator= (const FEMP &rhs);

  /**
   * Easy access to members of stored object
   */
  typename T::Pointer operator-> () const { return m_Data; }
  
  /**
   * Dereferencing operator provides automatic conversion from 
   * special to standard pointer to object
   */  
  operator T * () const
    {
    return m_Data;
    }
  
  /**
   * Return true if special pointer actually points
   * to a valid object and false otherwise.
   */
  bool IsNULL() const
    {
    return (m_Data==0);
    }

private:

  /**
   * Pointer to actual object. Note that this could be a SmartPointer.
   */
  typename T::Pointer m_Data;

};

template<class T>
const FEMP<T>& FEMP<T>::operator= (const FEMP &rhs)
{

  /** Self assignments don't make sense. */
  if (&rhs!=this) 
    {
    /**
     * First destroy the existing object on the left hand side
     */
    #ifndef FEM_USE_SMART_POINTERS
    delete m_Data;
    #else
    m_Data=0;
    #endif

    /**
     * Then clone the one on the right hand side
     * of the expression (if not NULL).
     */
    if (rhs.m_Data)
      {
      m_Data=static_cast<T*>(&*rhs.m_Data->Clone());
      }
    else
      {
      m_Data=0;
      }

    }
  return *this;
}

}} // end namespace itk::fem

#endif // #ifndef __itkFEMP_h
