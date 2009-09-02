/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkNormalVariateGenerator.cxx,v $
  Language:  C++
  Date:      $Date: 2009-05-02 05:43:58 $
  Version:   $Revision: 1.1 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#include <vnl/vnl_math.h>
#include <math.h>

#include "itkNormalVariateGenerator.h"

namespace itk {
namespace Statistics {

NormalVariateGenerator::NormalVariateGenerator()
{
  m_Scale = ((double) 30000000.0);
  m_Rscale = ((double) (1.0 / m_Scale));
  m_Rcons = ((double) (1.0 / (2.0 * 1024.0 * 1024.0 * 1024.0)));
  m_ELEN = 7;    /*  LEN must be 2 ** ELEN       */
  m_LEN = 128;
  m_LMASK = (4 * (m_LEN-1));
  m_TLEN  = (8*m_LEN);
  m_Vec1 = new int[m_TLEN];

  m_Gausssave = 0;
  this->Initialize(0);
}

NormalVariateGenerator::~NormalVariateGenerator()
{
  delete[] m_Vec1;
}

void NormalVariateGenerator::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);

  os << indent << "Scale: " << m_Scale << std::endl;
  os << indent << "Rscale: " << m_Rscale << std::endl;
  os << indent << "Rcons: " << m_Rcons << std::endl;
  os << indent << "ELEN: " << m_ELEN << std::endl;
  os << indent << "LEN: " << m_LEN << std::endl;
  os << indent << "LMASK: " << m_LMASK << std::endl;
  os << indent << "TLEN: " << m_TLEN << std::endl;
    
  os << indent << "gaussfaze: " << m_Gaussfaze << std::endl;
  os << indent << "gausssave: " << m_Gausssave << std::endl;
  os << indent << "GScale: " << m_GScale << std::endl;
    
  os << indent << "vec1: " << m_Vec1 << std::endl;
  os << indent << "nslew: " << m_Nslew << std::endl;
  os << indent << "irs: " << m_Irs << std::endl;
  os << indent << "lseed: " << m_Lseed << std::endl;
  os << indent << "chic1: " << m_Chic1 << std::endl;
  os << indent << "chic2: " << m_Chic2 << std::endl;
  os << indent << "actualRSD: " << m_ActualRSD << std::endl;
}

void NormalVariateGenerator::Initialize(int randomSeed)
{
  // m_Random Seed was originally getpid()

  double fake;
  m_Lseed = randomSeed;
  m_Irs = randomSeed;
  m_Gaussfaze = 1;
  m_Nslew = 0;
  m_GScale = m_Rscale;
//      At one stage, we need to generate a random variable Z such that
//      (TLEN * Z*Z) has a Chi-squared-TLEN density. Now, a var with
//      an approximate Chi-sq-K distn can be got as
//          0.5 * (C + A*n)**2  where n has unit Normal distn,
//      A = (1 + 1 / (8K)),  C*C = 2K - A*A    (For large K)
//          So we form Z as (sqrt (1 / 2TLEN)) * (C + A*n)
//      or:
//          Z = (sqrt (1/2TLEN)) * A * (B + n)
//      where:
//          B = C / A.
//      We set m_Chic1 = A * vcl_sqrt(0.5 / TLEN),  m_Chic2 = B
  
  fake = 1.0 + 0.125 / m_TLEN;   // This is A 
  m_Chic2 = vcl_sqrt(2.0 * m_TLEN  -  fake*fake) /  fake;
  m_Chic1 = fake * vcl_sqrt(0.5 / m_TLEN);

  m_ActualRSD = 0.0;
  return;
}


double NormalVariateGenerator::GetVariate()
{
  if (--m_Gaussfaze)
    return m_GScale * m_Gausssave[m_Gaussfaze];
  else
    return FastNorm();
}

/*      -----------------------------------------------------   */


double NormalVariateGenerator::FastNorm(void)
{
  int i;
  int inc    = 0;
  int skew;
  int stride;
  int mask   = 0;
  int p;
  int q;
  int r;
  int s;
  int t;
  int *pa = 0;
  int *pb = 0;
  int *pc = 0;
  int *pd = 0;
  int *pe;
  int *p0 = 0;
  int mtype;
  int stype;
  double ts;
  double tr;
  double tx;
  double ty;
  double tz;

  /*    See if time to make a new set of 'original' deviates  */
  /*    or at least to correct for a drift in sum-of-squares    */
  if (! (m_Nslew & 0xFF)) goto renormalize;

 startpass:
  /*    Count passes    */
  m_Nslew ++;
  /*    Reset index into Saved values   */
  m_Gaussfaze = m_TLEN - 1; /* We will steal the last one   */
  /*    Update pseudo-random and use to choose type of rotation  */
  m_Lseed = 69069 * m_Lseed + 33331;
  m_Irs = (m_Irs <= 0) ? ((m_Irs << 1) ^ 333556017):(m_Irs << 1);
  t = m_Irs + m_Lseed;
  if (t < 0) t = ~t;
  /*    This gives us 31 random bits in t       */
  /*    We need ELEN to fix initial index into LEN, ELEN-1 to fix an odd
        stride, 2 to fix matrix type and maybe 1 for scantype, making
        2*ELEN + 2 in all, and leaving 29 - 2*ELEN unused
  */
  t = t >> (29 - 2*m_ELEN);       /*  Discard unwanted digits  */
  skew = (m_LEN-1) & t;  t = t >> m_ELEN;
  skew = 4 * skew;      /*  To give a word index to group of 4 */
  stride = (m_LEN/2 -1 ) & t;     t = t >> (m_ELEN-1);
  stride = 8 * stride + 4;      /* To give an odd num of 4-groups */
  mtype = t & 3;
  /*    Leaves a bit for stype, but not currently used   */

  /*    Use last bits of m_Nslew to determine scanning pattern   */
  stype = m_Nslew & 3;
  switch (stype)
    {
    case 0:               /*   From consecutive in top to scattered in bot  */
      inc = 1;
      mask = m_LMASK;
      pa = m_Vec1;  pb = pa + m_LEN;  pc = pb + m_LEN;  pd = pc + m_LEN;
      p0 = m_Vec1 + 4 * m_LEN;
      goto scanset;
    case 1:               /*   From consec in bot to scatt in top  */
      inc = 1;
      mask = m_LMASK;
      pa = m_Vec1 + 4 * m_LEN;  pb = pa + m_LEN;  pc = pb + m_LEN;  pd = pc + m_LEN;
      p0 = m_Vec1;
      goto scanset;
    case 2:               /*   From consec in even to scatt in odd  */
      inc = 2;
      mask = 2*m_LMASK;   skew *= 2;   stride *= 2;
      pa = m_Vec1 + 1;  pb = pa + 2*m_LEN;  pc = pb + 2*m_LEN;  pd = pc + 2*m_LEN;
      p0 = m_Vec1;
      goto scanset;
    case 3:               /*  From consec in odd to scatt in even  */
      inc = 2;
      mask = 2*m_LMASK;   skew *= 2;   stride *= 2;
      pa = m_Vec1;  pb = pa + 2*m_LEN;  pc = pb + 2*m_LEN;  pd = pc + 2*m_LEN;
      p0 = m_Vec1 + 1;
      goto scanset;
    }     /*   End of scan pattern cases */

scanset:
  m_Gausssave = m_Vec1;
  /*    Set loop count  */
  i = m_LEN;

  /*    Use mtype to select matrix   */
  switch (mtype)
    {
    case 0:               goto matrix0;
    case 1:               goto matrix1;
    case 2:               goto matrix2;
    case 3:               goto matrix3;
    }

matrix0:
  pa += (inc * (m_LEN-1));
mpass0:
  skew = (skew + stride) & mask;
  pe = p0 + skew;
  p = -*pa;  q = -*pb;  r =  *pc;  s =  *pd;
  t = (p + q + r + s) >> 1;
  p = t - p;  q = t - q;  r = t - r;  s = t - s;
  /*    Have new values in p,q,r,s.  Place and save replaced vals  */
  t = -*pe;  *pe = p;   pe += inc;
  p = *pe;  *pe = q;   pe += inc;
  q = -*pe;  *pe = r;   pe += inc;
  r = *pe;  *pe = s;
  /*    Have vals in p,q,r,t    */
  s = (p + q + r + t) >> 1;
  *pa = s - p;   pa -= inc;
  *pb = s - q;   pb += inc;
  *pc = s - r;   pc += inc;
  *pd = s - t;   pd += inc;
  if (--i) goto mpass0;
  goto endpass;

 matrix1:
  pb += (inc * (m_LEN-1));
 mpass1:
  skew = (skew + stride) & mask;
  pe = p0 + skew;
  p = -*pa;  q = *pb;  r = *pc;  s = -*pd;
  t = (p + q + r + s) >> 1;
  p = t - p;  q = t - q;  r = t - r;  s = t - s;
  /*    Have new values in p,q,r,s.  Place and save replaced vals  */
  t = *pe;  *pe = p;   pe += inc;
  p = -*pe;  *pe = q;   pe += inc;
  q = -*pe;  *pe = r;   pe += inc;
  r = *pe;  *pe = s;
  /*    Have vals in p,q,r,t    */
  s = (p + q + r + t) >> 1;
  *pa = s - p;   pa += inc;
  *pb = s - t;   pb -= inc;
  *pc = s - q;   pc += inc;
  *pd = s - r;   pd += inc;
  if (--i) goto mpass1;
  goto endpass;

 matrix2:
  pc += (inc * (m_LEN-1));
 mpass2:
  skew = (skew + stride) & mask;
  pe = p0 + skew;
  p = *pa;  q = -*pb;  r = *pc;  s = -*pd;
  t = (p + q + r + s) >> 1;
  p = t - p;  q = t - q;  r = t - r;  s = t - s;
  /*    Have new values in p,q,r,s.  Place and save replaced vals  */
  t = *pe;  *pe = p;   pe += inc;
  p = *pe;  *pe = q;   pe += inc;
  q = -*pe;  *pe = r;   pe += inc;
  r = -*pe;  *pe = s;
  /*    Have vals in p,q,r,t    */
  s = (p + q + r + t) >> 1;
  *pa = s - r;   pa += inc;
  *pb = s - p;   pb += inc;
  *pc = s - q;   pc -= inc;
  *pd = s - t;   pd += inc;
  if (--i) goto mpass2;
  goto endpass;

 matrix3:
  pd += (inc * (m_LEN-1));
 mpass3:
  skew = (skew + stride) & mask;
  pe = p0 + skew;
  p = *pa;  q = *pb;  r = -*pc;  s = -*pd;
  t = (p + q + r + s) >> 1;
  p = t - p;  q = t - q;  r = t - r;  s = t - s;
  /*    Have new values in p,q,r,s.  Place and save replaced vals  */
  t = -*pe;  *pe = p;   pe += inc;
  p =  *pe;  *pe = q;   pe += inc;
  q =  *pe;  *pe = r;   pe += inc;
  r = -*pe;  *pe = s;
  /*    Have vals in p,q,r,t    */
  s = (p + q + r + t) >> 1;
  *pa = s - q;   pa += inc;
  *pb = s - r;   pb += inc;
  *pc = s - t;   pc += inc;
  *pd = s - p;   pd -= inc;
  if (--i) goto mpass3;
  goto endpass;

 endpass:
  /*    Choose a value for m_GScale which will make the sum-of-squares have
        the variance of Chi-Sq (TLEN), i.e., 2*TLEN.  Choose a value from
        Chi-Sq (TLEN) using the method descibed in initnorm.
        The Normal variate is obtained from m_Gausssave[TLEN-1], which is
        not used by the caller.
  */
  ts = m_Chic1 * (m_Chic2 + m_GScale * m_Vec1 [m_TLEN-1]);
  /*    m_TLEN * ts * ts  has ChiSq (m_TLEN) distribution   */
  m_GScale = m_Rscale * ts * m_ActualRSD;
  return (m_GScale * m_Vec1[0]);

 renormalize:
  if (m_Nslew & 0xFFFF) goto recalcsumsq;
  /*    Here, replace the whole pool with conventional Normal variates  */
  ts = 0.0;
  p = 0;
 nextpair:
  m_Lseed = 69069 * m_Lseed + 33331;
  m_Irs = (m_Irs <= 0) ? ((m_Irs << 1) ^ 333556017):(m_Irs << 1);
  r = m_Irs + m_Lseed;
  tx = m_Rcons * r;
  m_Lseed = 69069 * m_Lseed + 33331;
  m_Irs = (m_Irs <= 0) ? ((m_Irs << 1) ^ 333556017):(m_Irs << 1);
  r = m_Irs + m_Lseed;
  ty = m_Rcons * r;
  tr = tx * tx + ty * ty;
  if ((tr > 1.0) || (tr < 0.1)) goto nextpair;
  m_Lseed = 69069 * m_Lseed + 33331;
  m_Irs = (m_Irs <= 0) ? ((m_Irs << 1) ^ 333556017):(m_Irs << 1);
  r = m_Irs + m_Lseed;
  if (r < 0) r = ~r;
  tz = -2.0 * vcl_log((r + 0.5) * m_Rcons);   /* Sum of squares */
  ts += tz;
  tz = vcl_sqrt(tz / tr );
  m_Vec1 [p++] = (int) (m_Scale *  tx * tz);
  m_Vec1 [p++] = (int) (m_Scale *  ty * tz);
  if (p < m_TLEN) goto nextpair;
  /*    Horrid, but good enough */
  /*    Calc correction factor to make sum of squares = TLEN    */
  ts = m_TLEN / ts;  /* Should be close to 1.0  */
  tr = vcl_sqrt(ts);
  for (p = 0; p < m_TLEN; p++)
    {
    tx = m_Vec1 [p] * tr;
    m_Vec1 [p] = (int) ((tx < 0.0) ? (tx - 0.5) : (tx + 0.5));
    }

 recalcsumsq:
  /*    Calculate actual sum of squares for correction   */
  ts = 0.0;
  for (p = 0; p < m_TLEN; p++)
    {
    tx = m_Vec1[p];
    ts += (tx * tx);
    }
  /*    Now ts should be Scale*Scale*TLEN or thereabouts   */
  ts = vcl_sqrt(ts / (m_Scale * m_Scale * m_TLEN));
  m_ActualRSD = 1.0 / ts;   /* Reciprocal of actual Standard Devtn */
  goto startpass;

}

} // end of name space Statistics
} // end of name space itk
