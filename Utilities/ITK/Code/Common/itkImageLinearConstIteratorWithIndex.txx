/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkImageLinearConstIteratorWithIndex.txx,v $
  Language:  C++
  Date:      $Date: 2005/07/13 13:54:35 $
  Version:   $Revision: 1.18 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef _itkImageLinearConstIteratorWithIndex_txx
#define _itkImageLinearConstIteratorWithIndex_txx

#include "itkImageLinearConstIteratorWithIndex.h"


namespace itk
{



//----------------------------------------------------------------------
//  Constructor
//----------------------------------------------------------------------
template<class TImage>
ImageLinearConstIteratorWithIndex<TImage>
::ImageLinearConstIteratorWithIndex(
                          const ImageType *ptr,
                          const RegionType& region )
    : ImageConstIteratorWithIndex<TImage>( ptr, region ) 
{
  this->SetDirection( 0 );
}



//----------------------------------------------------------------------
//  Go to next line
//----------------------------------------------------------------------
template<class TImage>
void 
ImageLinearConstIteratorWithIndex<TImage>
::NextLine(void)
{

  this->m_Position -=   this->m_OffsetTable[ m_Direction ] * 
    ( this->m_PositionIndex[ m_Direction ] - this->m_BeginIndex[ m_Direction ] ); 
  
  this->m_PositionIndex[m_Direction] = this->m_BeginIndex[m_Direction];     

  for( unsigned int n=0; n<TImage::ImageDimension; n++ )
    {

    this->m_Remaining = false;
    
    if( n == m_Direction ) 
      {
      continue;
      }
    
    this->m_PositionIndex[ n  ]++;
    if( this->m_PositionIndex[n] <  this->m_EndIndex[n] )
      {
      this->m_Position += this->m_OffsetTable[ n ];
      this->m_Remaining = true;
      break;
      }
    else 
      {
      this->m_Position -= this->m_OffsetTable[ n ] * ( this->m_Region.GetSize()[n]-1 );
      this->m_PositionIndex[ n ] = this->m_BeginIndex[ n ]; 
      }
    }
}



//----------------------------------------------------------------------
//  Pass to the last pixel on the previous line
//----------------------------------------------------------------------
template<class TImage>
void 
ImageLinearConstIteratorWithIndex<TImage>
::PreviousLine(void)
{

  this->m_Position +=   this->m_OffsetTable[ m_Direction ] * 
    ( this->m_EndIndex[ m_Direction ] - 1 - this->m_PositionIndex[ m_Direction ] ); 
  
  this->m_PositionIndex[m_Direction] = this->m_EndIndex[m_Direction]-1;   


  for( unsigned int n=0; n<TImage::ImageDimension; n++ )
    {

    this->m_Remaining = false;
    
    if( n == m_Direction ) 
      {
      continue;
      }
    
    this->m_PositionIndex[ n  ]--;
    if( this->m_PositionIndex[ n ] >=  this->m_BeginIndex[ n ] )
      {
      this->m_Position -= this->m_OffsetTable[ n ];
      this->m_Remaining = true;
      break;
      }
    else 
      {
      this->m_Position += this->m_OffsetTable[ n ] * ( this->m_Region.GetSize()[n]-1 );
      this->m_PositionIndex[ n ] = this->m_EndIndex[ n ] - 1; 
      }
    }
}





//----------------------------------------------------------------------
//  Test for end of line
//----------------------------------------------------------------------
template<class TImage>
bool 
ImageLinearConstIteratorWithIndex<TImage>
::IsAtEndOfLine(void) 
{
  return this->m_PositionIndex[m_Direction] >= this->m_EndIndex[m_Direction];
}


//----------------------------------------------------------------------
//  Go to the last pixel of the current line
//----------------------------------------------------------------------
template<class TImage>
void 
ImageLinearConstIteratorWithIndex<TImage>
::GoToReverseBeginOfLine(void)
{
  long distanceToEnd = 
    this->m_EndIndex[ m_Direction ] - this->m_PositionIndex[ m_Direction ] - 1;
  this->m_Position += m_Jump * distanceToEnd; 
  this->m_PositionIndex[m_Direction] = this->m_EndIndex[m_Direction];
}



//----------------------------------------------------------------------
//  Test for begin of line
//----------------------------------------------------------------------
template<class TImage>
bool 
ImageLinearConstIteratorWithIndex<TImage>
::IsAtReverseEndOfLine(void) 
{
  return this->m_PositionIndex[m_Direction] < this->m_BeginIndex[m_Direction];
}




//----------------------------------------------------------------------
//  Go to the first pixel of the current line
//----------------------------------------------------------------------
template<class TImage>
void 
ImageLinearConstIteratorWithIndex<TImage>
::GoToBeginOfLine(void)
{

  long distanceToBegin = 
    this->m_PositionIndex[ m_Direction ] - this->m_BeginIndex[ m_Direction ];

  this->m_Position -= m_Jump * distanceToBegin; 

  this->m_PositionIndex[m_Direction] = this->m_BeginIndex[m_Direction];   
  
}




//----------------------------------------------------------------------
//  Pass to the past last pixel of the current line
//----------------------------------------------------------------------
template<class TImage>
void 
ImageLinearConstIteratorWithIndex<TImage>
::GoToEndOfLine(void)
{

  long distanceToEnd = 
    this->m_EndIndex[ m_Direction ] - this->m_PositionIndex[ m_Direction ];


  this->m_Position += m_Jump * distanceToEnd; 

  this->m_PositionIndex[m_Direction] = this->m_EndIndex[m_Direction];
  
}






//----------------------------------------------------------------------
//  Set direction of movement
//----------------------------------------------------------------------
template<class TImage>
void 
ImageLinearConstIteratorWithIndex<TImage>
::SetDirection(unsigned int direction) 
{
  if( direction >= TImage::ImageDimension )
    {
    itkGenericExceptionMacro(<<"In image of dimension " << TImage::ImageDimension <<" Direction " << direction << " sas selected");
    }
  m_Direction = direction;
  m_Jump = this->m_OffsetTable[ m_Direction ];
}
 




//----------------------------------------------------------------------
//  Advance along the line
//----------------------------------------------------------------------
template<class TImage>
ImageLinearConstIteratorWithIndex<TImage>  & 
ImageLinearConstIteratorWithIndex<TImage>
::operator++()
{
  this->m_PositionIndex[m_Direction]++;
  this->m_Position += m_Jump;
  return *this;
}




//----------------------------------------------------------------------
//  Advance along the line backwards
//----------------------------------------------------------------------
template<class TImage>
ImageLinearConstIteratorWithIndex<TImage>  & 
ImageLinearConstIteratorWithIndex<TImage>
::operator--()
{
  this->m_PositionIndex[m_Direction]--;
  this->m_Position -= m_Jump;
  return *this;
}




} // end namespace itk

#endif
