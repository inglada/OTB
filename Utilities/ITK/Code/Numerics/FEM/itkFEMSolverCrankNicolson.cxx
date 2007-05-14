/*=========================================================================

  Program:   Insight Segmentation & Registration Toolkit
  Module:    $RCSfile: itkFEMSolverCrankNicolson.cxx,v $
  Language:  C++
  Date:      $Date: 2006/03/19 04:37:20 $
  Version:   $Revision: 1.38 $

  Copyright (c) Insight Software Consortium. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/

// disable debug warnings in MS compiler
#ifdef _MSC_VER
#pragma warning(disable: 4786)
#endif

#include "itkFEMSolverCrankNicolson.h"

#include "itkFEMLoadNode.h"
#include "itkFEMLoadElementBase.h"
#include "itkFEMLoadBC.h"
#include "itkFEMLoadBCMFC.h"
#include "itkFEMLoadLandmark.h"

namespace itk {
namespace fem {

#define TOTE

void SolverCrankNicolson::InitializeForSolution() 
{
  m_ls->SetSystemOrder(NGFN+NMFC);
  m_ls->SetNumberOfVectors(6);
  m_ls->SetNumberOfSolutions(3);
  m_ls->SetNumberOfMatrices(2);
  m_ls->InitializeMatrix(SumMatrixIndex);
  m_ls->InitializeMatrix(DifferenceMatrixIndex);
  m_ls->InitializeVector(ForceTIndex);
  m_ls->InitializeVector(ForceTotalIndex);
  m_ls->InitializeVector(ForceTMinus1Index);
  m_ls->InitializeVector(SolutionVectorTMinus1Index);
  m_ls->InitializeVector(DiffMatrixBySolutionTMinus1Index);
  m_ls->InitializeSolution(SolutionTIndex);
  m_ls->InitializeSolution(TotalSolutionIndex);
  m_ls->InitializeSolution(SolutionTMinus1Index);
}

/*
 * Assemble the master stiffness matrix (also apply the MFCs to K)
 */  
void SolverCrankNicolson::AssembleKandM() 
{

  // if no DOFs exist in a system, we have nothing to do
  if (NGFN<=0) return;

  Float lhsval;
  Float rhsval;
  NMFC=0;  // number of MFC in a system

  // temporary storage for pointers to LoadBCMFC objects
  typedef std::vector<LoadBCMFC::Pointer> MFCArray;
  MFCArray mfcLoads;

  /*
   * Before we can start the assembly procedure, we need to know,
   * how many boundary conditions (MFCs) there are in a system.
   */
  mfcLoads.clear();
  // search for MFC's in Loads array, because they affect the master stiffness matrix
  for(LoadArray::iterator l=load.begin(); l!=load.end(); l++) {
    if ( LoadBCMFC::Pointer l1=dynamic_cast<LoadBCMFC*>( &(*(*l))) ) {
      // store the index of an LoadBCMFC object for later
      l1->Index=NMFC;
      // store the pointer to a LoadBCMFC object for later
      mfcLoads.push_back(l1);
      // increase the number of MFC
      NMFC++;
    }
  }
 
  /*
   * Now we can assemble the master stiffness matrix
   * from element stiffness matrices
   */
  InitializeForSolution(); 
  
 
  /*
   * Step over all elements
   */
  for(ElementArray::iterator e=el.begin(); e!=el.end(); e++)
  {
    vnl_matrix<Float> Ke;
    (*e)->GetStiffnessMatrix(Ke);  /*Copy the element stiffness matrix for faster access. */
    vnl_matrix<Float> Me;
    (*e)->GetMassMatrix(Me);  /*Copy the element mass matrix for faster access. */
    int Ne=(*e)->GetNumberOfDegreesOfFreedom();          /*... same for element DOF */

    Me=Me*m_rho;

    /* step over all rows in in element matrix */
    for(int j=0; j<Ne; j++)
    {
      /* step over all columns in in element matrix */
      for(int k=0; k<Ne; k++) 
      {
        /* error checking. all GFN should be =>0 and <NGFN */
        if ( (*e)->GetDegreeOfFreedom(j) >= NGFN ||
             (*e)->GetDegreeOfFreedom(k) >= NGFN  )
        {
          throw FEMExceptionSolution(__FILE__,__LINE__,"SolverCrankNicolson::AssembleKandM()","Illegal GFN!");
        }
        
        /* Here we finaly update the corresponding element
         * in the master stiffness matrix. We first check if 
         * element in Ke is zero, to prevent zeros from being 
         * allocated in sparse matrix.
         */
        if ( Ke(j,k)!=Float(0.0) || Me(j,k) != Float(0.0) )
        {
          // left hand side matrix
          lhsval=(Me(j,k) + m_alpha*m_deltaT*Ke(j,k));
          m_ls->AddMatrixValue( (*e)->GetDegreeOfFreedom(j) , 
                    (*e)->GetDegreeOfFreedom(k), 
                    lhsval, SumMatrixIndex );
          // right hand side matrix
          rhsval=(Me(j,k) - (1.-m_alpha)*m_deltaT*Ke(j,k));
          m_ls->AddMatrixValue( (*e)->GetDegreeOfFreedom(j) , 
                    (*e)->GetDegreeOfFreedom(k), 
                    rhsval, DifferenceMatrixIndex );
//          if (k == j ) std::cout << " ke " << Ke(j,k) << " me " << Me(j,k) << std::endl;
        }
      }

    }

  }

  /*
   * Step over all the loads to add the landmark contributions to the
   * appropriate place in the stiffness matrix
   */
  for(LoadArray::iterator l2=load.begin(); l2!=load.end(); l2++) {
    if ( LoadLandmark::Pointer l3=dynamic_cast<LoadLandmark*>( &(*(*l2))) ) {
      Element::Pointer ep = const_cast<Element*>( l3->el[0] );

      Element::MatrixType Le;
      ep->GetLandmarkContributionMatrix( l3->eta, Le );
      
      int Ne = ep->GetNumberOfDegreesOfFreedom();
      
      // step over all rows in element matrix
      for (int j=0; j<Ne; j++) { 
        // step over all columns in element matrix
          for (int k=0; k<Ne; k++) {
            // error checking, all GFN should be >=0 and < NGFN
            if ( ep->GetDegreeOfFreedom(j) >= NGFN ||
                 ep->GetDegreeOfFreedom(k) >= NGFN ) {
                throw FEMExceptionSolution(__FILE__,__LINE__,"SolverCrankNicolson::AssembleKandM()","Illegal GFN!");
            }
    
          // Now update the corresponding element in the master
          // stiffness matrix and omit the zeros for the sparseness
          if ( Le(j,k)!=Float(0.0) ) {
            // lhs matrix
            lhsval = m_alpha*m_deltaT*Le(j,k);
            m_ls->AddMatrixValue( ep->GetDegreeOfFreedom(j),
                                ep->GetDegreeOfFreedom(k),
                                lhsval, SumMatrixIndex );
            //rhs matrix
            rhsval = (1.-m_alpha)*m_deltaT*Le(j,k);
            m_ls->AddMatrixValue( ep->GetDegreeOfFreedom(j),
                                ep->GetDegreeOfFreedom(k),
                                rhsval, DifferenceMatrixIndex );
            }
        }
      }
    }
  }

  /* step over all types of BCs */
  this->ApplyBC();  // BUG  -- are BCs applied appropriately to the problem?
}


/*
 * Assemble the master force vector
 */
void SolverCrankNicolson::AssembleFforTimeStep(int dim) {
/* if no DOFs exist in a system, we have nothing to do */
  if (NGFN<=0) return;
  AssembleF(dim); // assuming assemblef uses index 0 in vector!

  typedef std::map<Element::DegreeOfFreedomIDType,Float> BCTermType;
  BCTermType bcterm;

   /* Step over all Loads */
  for(LoadArray::iterator l=load.begin(); l!=load.end(); l++)
  {
    Load::Pointer l0=*l;
    if ( LoadBC::Pointer l1=dynamic_cast<LoadBC*>(&*l0) )
    {
      bcterm[ l1->m_element->GetDegreeOfFreedom(l1->m_dof) ]=l1->m_value[dim];
    }
  } // end for LoadArray::iterator l

  // Now set the solution t_minus1 vector to fit the BCs
  for( BCTermType::iterator q=bcterm.begin(); q!=bcterm.end(); q++)
  { 
    m_ls->SetVectorValue(q->first,0.0,SolutionVectorTMinus1Index); //FIXME? 
    m_ls->SetSolutionValue(q->first,0.0,SolutionTMinus1Index); //FIXME? 
    m_ls->SetSolutionValue(q->first,0.0,TotalSolutionIndex);
  }

  m_ls->MultiplyMatrixVector(DiffMatrixBySolutionTMinus1Index,
                             DifferenceMatrixIndex,SolutionVectorTMinus1Index);
   
  for (unsigned int index=0; index<NGFN; index++) RecomputeForceVector(index);

  // Now set the solution and force vector to fit the BCs
  for( BCTermType::iterator q=bcterm.begin(); q!=bcterm.end(); q++)
  { 
    m_ls->SetVectorValue(q->first,q->second,ForceTIndex); 
  }
}



void  SolverCrankNicolson::RecomputeForceVector(unsigned int index)
{// 
  Float ft   = m_ls->GetVectorValue(index,ForceTIndex);
  Float ftm1 = m_ls->GetVectorValue(index,ForceTMinus1Index);
  Float utm1 = m_ls->GetVectorValue(index,DiffMatrixBySolutionTMinus1Index);
  Float f=m_deltaT*(m_alpha*ft+(1.-m_alpha)*ftm1)+utm1;
  m_ls->SetVectorValue(index , f, ForceTIndex);
}



/*
 * Solve for the displacement vector u
 */  
void SolverCrankNicolson::Solve() 
{
 /* FIXME - must verify that this is correct use of wrapper */
  /* FIXME Initialize the solution vector */
  m_ls->InitializeSolution(SolutionTIndex);
  m_ls->Solve();  
// call this externally    AddToDisplacements(); 
}


void SolverCrankNicolson::FindBracketingTriplet(Float* a, Float* b, Float* c)
{
 // in 1-D domain, we want to find a < b < c , s.t.  f(b) < f(a) && f(b) < f(c)
 //  see Numerical Recipes
 
  Float Gold=1.618034;
  Float Glimit=100.0;
  Float Tiny=1.e-20;
  
  Float ax, bx,cx;
  ax=0.0; bx=1.;
  Float fc;
  Float fa=vcl_fabs(EvaluateResidual(ax));
  Float fb=vcl_fabs(EvaluateResidual(bx));
  
  Float ulim,u,r,q,fu,dum;

  if ( fb > fa ) 
  {
     dum=ax; ax=bx; bx=dum;
     dum=fb; fb=fa; fa=dum;
  }

  cx=bx+Gold*(bx-ax);  // first guess for c - the 3rd pt needed to bracket the min
  fc=vcl_fabs(EvaluateResidual(cx));

  
  while (fb > fc  /*&& vcl_fabs(ax) < 3. && vcl_fabs(bx) < 3. && vcl_fabs(cx) < 3.*/)
    {
    r=(bx-ax)*(fb-fc);
    q=(bx-cx)*(fb-fa);
    Float denom=(2.0*GSSign(GSMax(fabs(q-r),Tiny),q-r));
    u=(bx)-((bx-cx)*q-(bx-ax)*r)/denom;
    ulim=bx + Glimit*(cx-bx);
    if ((bx-u)*(u-cx) > 0.0)
      {
      fu=vcl_fabs(EvaluateResidual(u));
      if (fu < fc)
        {
        ax=bx;
        bx=u;
        *a=ax; *b=bx; *c=cx;
        return;
        }
      else if (fu > fb)
        {
        cx=u;
        *a=ax; *b=bx; *c=cx;
        return;
        }
      
        u=cx+Gold*(cx-bx);
        fu=vcl_fabs(EvaluateResidual(u));
        
      }
    else if ( (cx-u)*(u-ulim) > 0.0)
      {
      fu=vcl_fabs(EvaluateResidual(u));
      if (fu < fc)
        {
        bx=cx; cx=u; u=cx+Gold*(cx-bx);
        fb=fc; fc=fu; fu=vcl_fabs(EvaluateResidual(u));
        }
      
      }
    else if ( (u-ulim)*(ulim-cx) >= 0.0)
      {
      u=ulim;
      fu=vcl_fabs(EvaluateResidual(u));
      }
    else
      {
      u=cx+Gold*(cx-bx);
      fu=vcl_fabs(EvaluateResidual(u));
      }
    
    ax=bx; bx=cx; cx=u;
    fa=fb; fb=fc; fc=fu;
    
  }

  if ( vcl_fabs(ax) > 1.e3  || vcl_fabs(bx) > 1.e3 || vcl_fabs(cx) > 1.e3)
  { ax=-2.0;  bx=1.0;  cx=2.0; } // to avoid crazy numbers caused by bad bracket (u goes nuts)
  
  *a=ax; *b=bx; *c=cx;
}

Element::Float SolverCrankNicolson::BrentsMethod(Float tol,unsigned int MaxIters)
{
  // We should now have a, b and c, as well as f(a), f(b), f(c), 
  // where b gives the minimum energy position;

  Float CGOLD = 0.3819660;
  Float ZEPS = 1.e-10;

  Float ax=0.0, bx=1.0, cx=2.0;

  FindBracketingTriplet(&ax, &bx, &cx);

  Float xmin;

  unsigned int iter;
  
  Float a,b,d=0.,etemp,fu,fv,fw,fx,p,q,r,tol1,tol2,u,v,w,x,xm;

  Float e=0.0;  // the distance moved on the step before last;

  a=((ax  < cx) ? ax : cx);
  b=((ax  > cx) ? ax : cx);
  
  x=w=v=bx;
  fw=fv=fx=vcl_fabs(EvaluateResidual(x));

  for (iter = 1; iter <=MaxIters; iter++)
  {
    xm=0.5*(a+b);
    tol2=2.0*(tol1=tol*vcl_fabs(x)+ZEPS);
    if (fabs(x-xm) <= (tol2-0.5*(b-a)))
    {
      xmin=x;
      SetEnergyToMin(xmin);
      return fx;
    }

    if (fabs(e) > tol1){
      r=(x-w)*(fx-fv);
      q=(x-v)*(fx-fw);
      p=(x-v)*q-(x-w)*r;
      q=2.0*(q-r);
      if (q>0.0) p = -1.*p;
      q=vcl_fabs(q);
      etemp=e;
      e=d;
      if (fabs(p) >= vcl_fabs(0.5*q*etemp) || p <= q*(a-x) || p >= q*(b-x))
         d=CGOLD*(e=(x>=xm ? a-x : b-x));
      else{
        if (q == 0.0) q=q +ZEPS;
        d=p/q;
        u=x+d;
        if (u-a < tol2 || b-u < tol2) d=GSSign(tol1,xm-x); 
      }
  }else{
      d=CGOLD*(e=(x>= xm ? a-x : b-x));
    }
  
    u=(fabs(d) >= tol1 ? x+d : x + GSSign(tol1,d));
    fu=vcl_fabs(EvaluateResidual(u));
    if (fu <= fx){
      if ( u >= x ) a=x; else b=x;
      v=w; w=x;x=u;
      fv=fw; fw=fx; fx=fu;
    } else {
      if (u<x) a = u; else b=u;
      if (fu <= fw || w ==x) {
        v=w;
        w=u;
        fv=fw;
        fw=fu;
      } else if (fu <= fv || v==x || v == w) {
        v=u;
        fv=fu;
      }
    }
  }
  xmin=x;
  SetEnergyToMin(xmin);
  return fx;
}



Element::Float SolverCrankNicolson::GoldenSection(Float tol,unsigned int MaxIters)
{
  // We should now have a, b and c, as well as f(a), f(b), f(c), 
  // where b gives the minimum energy position;

  Float ax, bx, cx;


  FindBracketingTriplet(&ax, &bx, &cx);
  Float xmin,fmin;
  //if (fb!=0.0)
  Float f1,f2,x0,x1,x2,x3;

  Float R=0.6180339;
  Float C=(1.0-R);

  x0=ax;
  x3=cx;
  if (fabs(cx-bx) > vcl_fabs(bx-ax)){
    x1=bx;
    x2=bx+C*(cx-bx);
  } else {
    x2=bx;
    x1=bx-C*(bx-ax);
  }
  f1=vcl_fabs(EvaluateResidual(x1));
  f2=vcl_fabs(EvaluateResidual(x2));
  unsigned int iters=0;
  while (fabs(x3-x0) > tol*(fabs(x1)+vcl_fabs(x2)) && iters < MaxIters)
  {
    iters++;
    if (f2 < f1){
      x0=x1; x1=x2; x2=R*x1+C*x3;
      f1=f2; f2=vcl_fabs(EvaluateResidual(x2));
    } else {
      x3=x2; x2=x1; x1=R*x2+C*x0;
      f2=f1; f1=vcl_fabs(EvaluateResidual(x1));
    }
  }
  if (f1<f2){
    xmin=x1;
    fmin=f1;
  } else {
    xmin=x2;
    fmin=f2;
  }
  
  SetEnergyToMin(xmin);
  return fmin; 
}



void SolverCrankNicolson::SetEnergyToMin(Float xmin)
{
  for (unsigned int j=0; j<NGFN; j++)
    {
    Float SolVal;
    Float FVal;
#ifdef LOCE
    SolVal=xmin*m_ls->GetSolutionValue(j,SolutionTIndex)
      +(1.-xmin)*m_ls->GetSolutionValue(j,SolutionTMinus1Index);   
  
    FVal=xmin*m_ls->GetVectorValue(j,ForceTIndex)
      +(1.-xmin)*m_ls->GetVectorValue(j,ForceTMinus1Index);
#endif
#ifdef TOTE
    SolVal=xmin*m_ls->GetSolutionValue(j,SolutionTIndex);// FOR TOT E
    FVal=xmin*m_ls->GetVectorValue(j,ForceTIndex);
#endif 
    m_ls->SetSolutionValue(j,SolVal,SolutionTIndex);
    m_ls->SetVectorValue(j,FVal,ForceTIndex);
  }

}

Element::Float SolverCrankNicolson::GetDeformationEnergy(Float t)
{
  Float DeformationEnergy=0.0;
  Float iSolVal,jSolVal;

  for (unsigned int i=0; i<NGFN; i++)
    {
// forming  U^T F
#ifdef LOCE
    iSolVal=t*(m_ls->GetSolutionValue(i,SolutionTIndex))
      +(1.-t)*m_ls->GetSolutionValue(i,SolutionTMinus1Index);
#endif
#ifdef TOTE
    iSolVal=t*(m_ls->GetSolutionValue(i,SolutionTIndex))
      +m_ls->GetSolutionValue(i,TotalSolutionIndex);// FOR TOT E
#endif
// forming U^T K U
    Float TempRowVal=0.0;
    for (unsigned int j=0; j<NGFN; j++)
      {
#ifdef LOCE
      jSolVal=t*(m_ls->GetSolutionValue(j,SolutionTIndex))
        +(1.-t)*m_ls->GetSolutionValue(j,SolutionTMinus1Index);
#endif
#ifdef TOTE
      jSolVal=t*(m_ls->GetSolutionValue(j,SolutionTIndex))
        +m_ls->GetSolutionValue(j,TotalSolutionIndex);// FOR TOT E
#endif
      TempRowVal+=m_ls->GetMatrixValue(i,j,SumMatrixIndex)*jSolVal;
      }
    DeformationEnergy+=iSolVal*TempRowVal;
  }
  return DeformationEnergy;
}


Element::Float SolverCrankNicolson::EvaluateResidual(Float t)
{
 
  Float ForceEnergy=0.0,FVal=0.0;
  Float DeformationEnergy=0.0;
  Float iSolVal,jSolVal;

  for (unsigned int i=0; i<NGFN; i++)
  {
// forming  U^T F
#ifdef LOCE
    iSolVal=t*(m_ls->GetSolutionValue(i,SolutionTIndex))
       +(1.-t)*m_ls->GetSolutionValue(i,SolutionTMinus1Index);
    FVal=m_ls->GetVectorValue(i,ForceTIndex);
  FVal=t*FVal+(1.-t)*m_ls->GetVectorValue(i,ForceTMinus1Index);

    ForceEnergy+=iSolVal*FVal;
#endif
#ifdef TOTE
    FVal=FVal+0.0;
    iSolVal=t*(m_ls->GetSolutionValue(i,SolutionTIndex))
        +m_ls->GetSolutionValue(i,TotalSolutionIndex);// FOR TOT E
    ForceEnergy+=iSolVal*(m_ls->GetVectorValue(i,ForceTotalIndex)+
    t*m_ls->GetVectorValue(i,ForceTIndex));// FOR TOT E
#endif
// forming U^T K U
    Float TempRowVal=0.0;
    for (unsigned int j=0; j<NGFN; j++)
    {
#ifdef LOCE
      jSolVal=t*(m_ls->GetSolutionValue(j,SolutionTIndex))
         +(1.-t)*m_ls->GetSolutionValue(j,SolutionTMinus1Index);
#endif
#ifdef TOTE
      jSolVal=t*(m_ls->GetSolutionValue(j,SolutionTIndex))
       +m_ls->GetSolutionValue(j,TotalSolutionIndex);// FOR TOT E
#endif
      TempRowVal+=m_ls->GetMatrixValue(i,j,SumMatrixIndex)*jSolVal;
    }
    DeformationEnergy+=iSolVal*TempRowVal;
  }
  Float Energy=(Float) vcl_fabs(DeformationEnergy-ForceEnergy);
  return Energy;
}

/*
 * Copy solution vector u to the corresponding nodal values, which are
 * stored in node objects). This is standard post processing of the solution.
 */  
void SolverCrankNicolson::AddToDisplacements(Float optimum) 
{
  /*
   * Copy the resulting displacements from 
   * solution vector back to node objects.
   */
  Float maxs=0.0,CurrentTotSolution,CurrentSolution,CurrentForce;
  Float mins2=0.0, maxs2=0.0;
  Float absmax=0.0;

  for(unsigned int i=0;i<NGFN;i++)
  {  
#ifdef TOTE
    CurrentSolution=m_ls->GetSolutionValue(i,SolutionTIndex);
#endif
    if (CurrentSolution < mins2 ){  
      mins2=CurrentSolution;
    }
    else if (CurrentSolution > maxs2 ) {
      maxs2=CurrentSolution;
    } 
    if (fabs(CurrentSolution) > absmax) absmax=vcl_fabs(CurrentSolution);

//  note: set rather than add - i.e. last solution of system not total solution  
#ifdef LOCE
    CurrentSolution=optimum*m_ls->GetSolutionValue(i,SolutionTIndex)
          +(1.-optimum)*m_ls->GetVectorValue(i,SolutionVectorTMinus1Index);
    CurrentForce=optimum*m_ls->GetVectorValue(i,ForceTIndex)
          +(1.-optimum)*m_ls->GetVectorValue(i,ForceTMinus1Index);
    m_ls->SetVectorValue(i,CurrentSolution,SolutionVectorTMinus1Index);   
    m_ls->SetSolutionValue(i,CurrentSolution,SolutionTMinus1Index);   
    m_ls->SetVectorValue(i , CurrentForce, ForceTMinus1Index); // now set t minus one force vector correctly
#endif
#ifdef TOTE
    CurrentSolution=optimum*CurrentSolution;
    CurrentForce=optimum*m_ls->GetVectorValue(i,ForceTIndex);
    m_ls->SetVectorValue(i,CurrentSolution,SolutionVectorTMinus1Index); // FOR TOT E   
    m_ls->SetSolutionValue(i,CurrentSolution,SolutionTMinus1Index);  // FOR TOT E 
    m_ls->SetVectorValue(i,CurrentForce,ForceTMinus1Index);
#endif

    m_ls->AddSolutionValue(i,CurrentSolution,TotalSolutionIndex);
    m_ls->AddVectorValue(i , CurrentForce, ForceTotalIndex);
    CurrentTotSolution=m_ls->GetSolutionValue(i,TotalSolutionIndex);
   
    if ( vcl_fabs(CurrentTotSolution) > maxs ) {
      maxs=vcl_fabs(CurrentTotSolution);
    }

    
  }  
 
  m_CurrentMaxSolution=absmax;
}

/*
 * Compute maximum and minimum solution values.
 */  
void SolverCrankNicolson::PrintMinMaxOfSolution() 
{
  /*
   * Copy the resulting displacements from 
   * solution vector back to node objects.
   */
  Float mins=0.0, maxs=0.0;
  Float mins2=0.0, maxs2=0.0;
  for(unsigned int i=0;i<NGFN;i++)
  {  
    Float CurrentSolution=m_ls->GetSolutionValue(i,SolutionTIndex);
    if (CurrentSolution < mins2 )  mins2=CurrentSolution;
    else if (CurrentSolution > maxs2 )  maxs2=CurrentSolution;
    CurrentSolution=m_ls->GetSolutionValue(i,TotalSolutionIndex);
    if (CurrentSolution < mins )  mins=CurrentSolution;
    else if (CurrentSolution > maxs )  maxs=CurrentSolution;
  }
}

/*
 * Copy solution vector u to the corresponding nodal values, which are
 * stored in node objects). This is standard post processing of the solution.
 */  
void SolverCrankNicolson::AverageLastTwoDisplacements(Float t) 
{
 
  Float maxs=0.0;
  for(unsigned int i=0;i<NGFN;i++)
  {  
    Float temp=m_ls->GetSolutionValue(i,SolutionTIndex);
    Float temp2=m_ls->GetSolutionValue(i,SolutionTMinus1Index);
    Float newsol=t*(temp)+(1.-t)*temp2;
    m_ls->SetSolutionValue(i,newsol,SolutionTMinus1Index);  
    m_ls->SetVectorValue(i,newsol,SolutionVectorTMinus1Index);  
    m_ls->SetSolutionValue(i,newsol,SolutionTIndex);    
    if ( newsol > maxs )  maxs=newsol;
  }  
}

void SolverCrankNicolson::ZeroVector(int which) 
{
  for(unsigned int i=0;i<NGFN;i++)
  {  
    m_ls->SetVectorValue(i,0.0,which);
  }
}

void SolverCrankNicolson::PrintDisplacements() 
{
  std::cout <<  " printing current displacements " << std::endl;
  for(unsigned int i=0;i<NGFN;i++)
  {  
    std::cout << m_ls->GetSolutionValue(i,TotalSolutionIndex) << std::endl;
  }
}

void SolverCrankNicolson::PrintForce() 
{
  std::cout <<  " printing current forces " << std::endl;
  for(unsigned int i=0;i<NGFN;i++)
  {  
    std::cout << m_ls->GetVectorValue(i,ForceTIndex) << std::endl;
  }
}




}} // end namespace itk::fem
