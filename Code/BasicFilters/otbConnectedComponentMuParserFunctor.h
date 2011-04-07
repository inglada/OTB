/*=========================================================================

 Program:   ORFEO Toolbox
 Language:  C++
 Date:      $Date$
 Version:   $Revision$


 Copyright (c) Centre National d'Etudes Spatiales. All rights reserved.
 See OTBCopyright.txt for details.

 Some parts of this code are derived from ITK. See ITKCopyright.txt
 for details.


 This software is distributed WITHOUT ANY WARRANTY; without even
 the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 PURPOSE.  See the above copyright notices for more information.

 =========================================================================*/

#ifndef __otbConnectedComponentMuParserFunctor_h
#define __otbConnectedComponentMuParserFunctor_h

#include "otbParser.h"
#include "otbMacro.h"
#include "itkMacro.h"


#include <vnl/algo/vnl_lsqr.h>
#include <vnl/vnl_sparse_matrix_linear_system.h>
#include <vnl/vnl_least_squares_function.h>


namespace otb
{
/** \class ConnectedComponentMuParserFunctor
 * \brief functor used as input to itk connected component segmentation module.
 *
 * This functor is based on the mathematical parser library muParser.
 * The built in functions and operators list is available at:
 * http://muparser.sourceforge.net/mup_features.html#idDef2
 *
 * image pixels :
 * p1bX, p2bX, where X denote band index.
 *
 * Specific Functor additional input :
 * distance : euclidian distance
 * spectralAngle : NOT IMPLEMENTED
 *
 * OTB additional functions:
 * ndvi(r, niri)
 *
 * OTB additional constants:
 * e - log2e - log10e - ln2 - ln10 - pi - euler
 *
 *
 * \sa Parser
 *
 */

/** \class ConnectedComponentMuParserFunctor
 *  \brief This functor use MuParser as criteria for itk connected component module
 */
  namespace Functor
  {

    template<class TInput>
    class ITK_EXPORT ConnectedComponentMuParserFunctor
    {
   
    public:
      typedef Parser ParserType;
      typedef ConnectedComponentMuParserFunctor Self;

      std::string GetNameOfClass()
      {
        return "ConnectedComponentMuParserFunctor";
      }

      inline bool operator()(const TInput &p1, const TInput &p2)
      {


        double value;

        if(p1.GetSize()!=m_NbOfBands)
          {
          this->SetNumberOfBands(p1.GetSize());
          }

        // we fill the buffer
        for(unsigned int i=0; i<m_NbOfBands; i++)
          {
          m_AImageP1[i]= static_cast<double> (p1[i]);
          m_AImageP2[i]= static_cast<double> (p2[i]);
          }

        m_Distance = 0.0;
        for(unsigned int i = 0; i<m_NbOfBands;++i)
          {
          m_Distance +=(p1[i]-p2[i])*(p1[i]-p2[i]);
          }
        m_Distance  = vcl_sqrt(m_Distance);

        // cast
        try
        {
          value = m_Parser->Eval();
        }
        catch(itk::ExceptionObject& err)
        {
          itkExceptionMacro(<< err);
        }

        return static_cast<bool> (value);
      }


      void SetExpression(const std::string expression)
      {
        m_Expression=expression;
        m_Parser->SetExpr(m_Expression);
      }

      /** Return the expression to be parsed */
      std::string GetExpression() const
      {
        return m_Expression;
      }

      void SetNumberOfBands(unsigned int NbOfBands)
      {

        m_NbOfBands=NbOfBands;
        std::ostringstream varName;


        m_AImageP1.resize(NbOfBands,0.0);
        m_AImageP2.resize(NbOfBands,0.0);

        for(unsigned int i=0;i<NbOfBands;i++)
          {
          varName << "p1b" << i+1 ;
          m_Parser->DefineVar(varName.str(), &(m_AImageP1[i]));
          varName.str("");
          varName << "p2b" << i+1 ;
          m_Parser->DefineVar(varName.str(), &(m_AImageP2[i]));
          varName.str("");
          }
        // customized data
        //m_NbVar++;
        //this->SetDataSize(m_NbVar);
        m_Parser->DefineVar("distance", &m_Distance);
        m_Parser->DefineVar("spectralAngle", &m_SpectralAngle);
        //this->SetVarName(m_NbVar-1,"spectralDistance");

      }

      ConnectedComponentMuParserFunctor()
      {
        m_Parser = ParserType::New();
        m_NbOfBands =0;
      };

      ~ConnectedComponentMuParserFunctor()
      {};

    protected:


    private:

      ConnectedComponentMuParserFunctor(const Self &);//purposely not implemented
      void operator =(const Self &);    //purposely not implemented

      std::string m_Expression;
      ParserType::Pointer m_Parser;
      std::vector<double> m_AImageP1;
      std::vector<double> m_AImageP2;
      double m_Distance;
      double m_SpectralAngle;
      std::vector<std::string > m_VarName;
      unsigned int m_NbOfBands;
      double m_ParserResult;

    };
  } // end of Functor namespace


}//end namespace otb


#endif
