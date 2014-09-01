#ifndef DT3_SCRIPTINGCOMPUTE
#define DT3_SCRIPTINGCOMPUTE
//==============================================================================
///	
///	File: ScriptingCompute.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/Math/ExpressionParser.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

//==============================================================================
/// This node can evaluate arbitrary expressions
//==============================================================================

class ScriptingCompute: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingCompute,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                    ScriptingCompute		(void);	
									ScriptingCompute		(const ScriptingCompute &rhs);
        ScriptingCompute &			operator =				(const ScriptingCompute &rhs);	
        virtual                     ~ScriptingCompute		(void);
    
        virtual void                archive                 (const std::shared_ptr<Archive> &archive);
		
	public:
		/// Called to initialize the object
		virtual void				initialize				(void);
		
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean					compute					(const PlugBase *plug);

        
		/// Set the equation
		/// \param eq equation string
        void                        set_eq_a1               (const std::string &eq);
    
		/// Get the equation
		/// \return eq equation string
        std::string                 eq_a1                   (void) const;

		/// Set the equation
		/// \param eq equation string
        void                        set_eq_a2               (const std::string &eq);

		/// Get the equation
		/// \return eq equation string
        std::string                 eq_a2                   (void) const;

		/// Set the equation
		/// \param eq equation string
        void                        set_eq_a3               (const std::string &eq);

		/// Get the equation
		/// \return eq equation string
        std::string                 eq_a3                   (void) const;

		/// Set the equation
		/// \param eq equation string
        void                        set_eq_a4               (const std::string &eq);

		/// Get the equation
		/// \return eq equation string
        std::string                 eq_a4                   (void) const;


		/// Set the equation
		/// \param eq equation string
        void                        set_eq_b1               (const std::string &eq);

		/// Get the equation
		/// \return eq equation string
        std::string                 eq_b1                   (void) const;

		/// Set the equation
		/// \param eq equation string
        void                        set_eq_b2               (const std::string &eq);

		/// Get the equation
		/// \return eq equation string
        std::string                 eq_b2                   (void) const;

		/// Set the equation
		/// \param eq equation string
        void                        set_eq_b3               (const std::string &eq);

		/// Get the equation
		/// \return eq equation string
        std::string                 eq_b3                   (void) const;

		/// Set the equation
		/// \param eq equation string
        void                        set_eq_b4               (const std::string &eq);

		/// Get the equation
		/// \return eq equation string
        std::string                 eq_b4                   (void) const;


		/// Set the equation
		/// \param eq equation string
        void                        set_eq_c1               (const std::string &eq);

		/// Get the equation
		/// \return eq equation string
        std::string                 eq_c1                   (void) const;

		/// Set the equation
		/// \param eq equation string
        void                        set_eq_c2               (const std::string &eq);

		/// Get the equation
		/// \return eq equation string
        std::string                 eq_c2                   (void) const;

		/// Set the equation
		/// \param eq equation string
        void                        set_eq_c3               (const std::string &eq);

		/// Get the equation
		/// \return eq equation string
        std::string                 eq_c3                   (void) const;

		/// Set the equation
		/// \param eq equation string
        void                        set_eq_c4               (const std::string &eq);

		/// Get the equation
		/// \return eq equation string
        std::string                 eq_c4                   (void) const;



	private:	
    
        void                        dirty_outs              (void);
        void                        set_variables           (ExpressionParser &p);
        
		Plug<DTfloat>				_in_a1;
		Plug<DTfloat>				_in_a2;
		Plug<DTfloat>				_in_a3;
		Plug<DTfloat>				_in_a4;

		Plug<DTfloat>				_in_b1;
		Plug<DTfloat>				_in_b2;
		Plug<DTfloat>				_in_b3;
		Plug<DTfloat>				_in_b4;

		Plug<DTfloat>				_in_c1;
		Plug<DTfloat>				_in_c2;
		Plug<DTfloat>				_in_c3;
		Plug<DTfloat>				_in_c4;
        
        
		Plug<DTfloat>				_out_a1;
		Plug<DTfloat>				_out_a2;
		Plug<DTfloat>				_out_a3;
		Plug<DTfloat>				_out_a4;

		Plug<DTfloat>				_out_b1;
		Plug<DTfloat>				_out_b2;
		Plug<DTfloat>				_out_b3;
		Plug<DTfloat>				_out_b4;
		
		Plug<DTfloat>				_out_c1;
		Plug<DTfloat>				_out_c2;
		Plug<DTfloat>				_out_c3;
		Plug<DTfloat>				_out_c4;
        
        
        std::string                 _eq_a1;
        std::string                 _eq_a2;
        std::string                 _eq_a3;
        std::string                 _eq_a4;

        std::string                 _eq_b1;
        std::string                 _eq_b2;
        std::string                 _eq_b3;
        std::string                 _eq_b4;

        std::string                 _eq_c1;
        std::string                 _eq_c2;
        std::string                 _eq_c3;
        std::string                 _eq_c4;
        
        ExpressionParser            _eq_a1_parser;
        ExpressionParser            _eq_a2_parser;
        ExpressionParser            _eq_a3_parser;
        ExpressionParser            _eq_a4_parser;

        ExpressionParser            _eq_b1_parser;
        ExpressionParser            _eq_b2_parser;
        ExpressionParser            _eq_b3_parser;
        ExpressionParser            _eq_b4_parser;

        ExpressionParser            _eq_c1_parser;
        ExpressionParser            _eq_c2_parser;
        ExpressionParser            _eq_c3_parser;
        ExpressionParser            _eq_c4_parser;
		
};

//==============================================================================
//==============================================================================

} // DT3

#endif
