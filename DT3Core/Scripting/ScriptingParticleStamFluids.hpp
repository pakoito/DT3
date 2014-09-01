#ifndef DT3_SCRIPTINGPARTICLESTAMFLUIDS
#define DT3_SCRIPTINGPARTICLESTAMFLUIDS
//==============================================================================
///	
///	File: ScriptingParticleStamFluids.hpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================

#include "DT3Core/Scripting/ScriptingBase.hpp"
#include "DT3Core/Types/Math/Rectangle.hpp"

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Forward declarations
//==============================================================================

class Particles;

//==============================================================================
/// Stam Fluids for Particle System.
//==============================================================================

class ScriptingParticleStamFluids: public ScriptingBase {
    public:
        DEFINE_TYPE(ScriptingParticleStamFluids,ScriptingBase)
		DEFINE_CREATE_AND_CLONE
		DEFINE_PLUG_NODE
         
                                        ScriptingParticleStamFluids     (void);	
										ScriptingParticleStamFluids     (const ScriptingParticleStamFluids &rhs);
        ScriptingParticleStamFluids &	operator =                      (const ScriptingParticleStamFluids &rhs);	
        virtual                         ~ScriptingParticleStamFluids    (void);
    
        virtual void                    archive                 (const std::shared_ptr<Archive> &archive);
		
		/// Object was added to a world
		/// world world that object was added to
        virtual void                    add_to_world            (World *world);

		/// Object was removed from a world
        virtual void                    remove_from_world       (void);

	public:
		/// Called to initialize the object
		virtual void                    initialize				(void);
				
		/// Computes the value of the node
		/// \param plug plug to compute
 		DTboolean                       compute					(const PlugBase *plug);
        
		/// Sets number of cells
		/// \param n number of cells
        void                            set_n                   (const DTint n);

		/// Returns number of cells
		/// return number of cells
        DTint                           n                       (void) const;

        /// Registered with world to tick this node
		/// \param dt delta time
        void                            tick                    (const DTfloat dt);

	private:
        //
        // Stam fluid solver
        //
        void    add_source  (DTfloat *x, DTfloat *s, DTfloat dt );
        void    set_bnd     (DTint b, DTfloat* x );
        void    lin_solve   (DTint b, DTfloat *x, DTfloat *x0, DTfloat a, DTfloat c );
        void    diffuse     (DTint b, DTfloat *x, DTfloat *x0, DTfloat diff, DTfloat dt );
        void    advect      (DTint b, DTfloat *d, DTfloat *d0, DTfloat *u, DTfloat *v, DTfloat dt );
        void    project     (DTfloat *u, DTfloat *v, DTfloat *p, DTfloat *div );
        void    dens_step   (DTfloat *x, DTfloat *x0, DTfloat *u, DTfloat *v, DTfloat diff, DTfloat dt );
        void    vel_step    (DTfloat *u, DTfloat *v, DTfloat *u0, DTfloat *v0, DTfloat visc, DTfloat dt );

        std::vector<DTfloat>                    _u;
        std::vector<DTfloat>                    _v;
        std::vector<DTfloat>                    _u_prev;
        std::vector<DTfloat>                    _v_prev;
        std::vector<DTfloat>                    _dens;
        std::vector<DTfloat>                    _dens_prev;

        DTint                                   _n;
        
        DTfloat                                 _diff;
        DTfloat                                 _visc;
        DTfloat                                 _particle_drag;
        
        Rectangle                               _rectangle;
            
		Plug<Vector2>                           _turbulence;
        DTfloat                                 _turbulence_timer;
			
		Plug<std::shared_ptr<Particles>>		_in;
		Plug<std::shared_ptr<Particles>>		_out;

};

//==============================================================================
//==============================================================================

} // DT3

#endif
