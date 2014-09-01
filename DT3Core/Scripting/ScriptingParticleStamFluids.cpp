//==============================================================================
///	
///	File: ScriptingParticleStamFluids.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================
//
// From Jos Stam's code: http://www.dgp.toronto.edu/people/stam/reality/Research/pub.html
//
//==============================================================================

#include "DT3Core/Scripting/ScriptingParticleStamFluids.hpp"
#include "DT3Core/System/Factory.hpp"
#include "DT3Core/System/Profiler.hpp"
#include "DT3Core/Types/FileBuffer/ArchiveData.hpp"
#include "DT3Core/Types/FileBuffer/Archive.hpp"
#include "DT3Core/Types/Graphics/Particles.hpp"
#include "DT3Core/Types/Math/Perlin.hpp"
#include "DT3Core/World/World.hpp"

//==============================================================================
//==============================================================================

#define IX(i,j) ((i)+(_n+2)*(j))
#define SWAP(x0,x) {DTfloat *tmp=x0;x0=x;x=tmp;}
#define FOR_EACH_CELL for ( i=1 ; i<=_n ; i++ ) { for ( j=1 ; j<=_n ; j++ ) {
#define END_FOR }}

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Register with object factory
//==============================================================================

IMPLEMENT_FACTORY_CREATION_SCRIPT(ScriptingParticleStamFluids,"Particles",NULL)
IMPLEMENT_PLUG_NODE(ScriptingParticleStamFluids)

IMPLEMENT_PLUG_INFO_INDEX(_turbulence)
IMPLEMENT_PLUG_INFO_INDEX(_in)
IMPLEMENT_PLUG_INFO_INDEX(_out)

//==============================================================================
//==============================================================================

BEGIN_IMPLEMENT_PLUGS(ScriptingParticleStamFluids)

	PLUG_INIT(_turbulence,"Turbulence")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));

	PLUG_INIT(_in,"In")
		.set_input(true)
		.affects(PLUG_INFO_INDEX(_out));
	
	PLUG_INIT(_out,"Out")
		.set_output(true);
        
END_IMPLEMENT_PLUGS

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

ScriptingParticleStamFluids::ScriptingParticleStamFluids (void)
    :   _rectangle      (0.0F,1024.0F,0.0F,768.0F),
        _n              (20),
        _diff           (1.0F),
        _visc           (1.0F),
        _particle_drag  (0.5F),
		_turbulence	(PLUG_INFO_INDEX(_turbulence), Vector2(0.0F,0.0F)),
        _turbulence_timer(0.0F),
		_in				(PLUG_INFO_INDEX(_in)),
		_out			(PLUG_INFO_INDEX(_out))
{  

}
		
ScriptingParticleStamFluids::ScriptingParticleStamFluids (const ScriptingParticleStamFluids &rhs)
    :   ScriptingBase	(rhs),
        _rectangle      (rhs._rectangle),
		_n              (rhs._n),
		_diff           (rhs._diff),
		_visc           (rhs._visc),
        _particle_drag  (rhs._particle_drag),
        _turbulence     (rhs._turbulence),
        _turbulence_timer(rhs._turbulence_timer),
		_in				(rhs._in),
		_out			(rhs._out)
{   

}

ScriptingParticleStamFluids & ScriptingParticleStamFluids::operator = (const ScriptingParticleStamFluids &rhs)
{
    // Make sure we are not assigning the class to itself
    if (&rhs != this) {        
		ScriptingBase::operator = (rhs);

        _rectangle = rhs._rectangle;
        
		_n = rhs._n;
		_diff = rhs._diff;
		_visc = rhs._visc;
        _particle_drag = rhs._particle_drag;
        
        _turbulence = rhs._turbulence;
        _turbulence_timer = rhs._turbulence_timer;
        
		_in	= rhs._in;
		_out = rhs._out;
	}
    return (*this);
}
			
ScriptingParticleStamFluids::~ScriptingParticleStamFluids (void)
{

}

//==============================================================================
//==============================================================================

void ScriptingParticleStamFluids::initialize (void)
{
	ScriptingBase::initialize();
}

//==============================================================================
//==============================================================================

void ScriptingParticleStamFluids::archive (const std::shared_ptr<Archive> &archive)
{
    ScriptingBase::archive(archive);

	archive->push_domain (class_id ());
	
	*archive << ARCHIVE_DATA_ACCESSORS("n", ScriptingParticleStamFluids::n, ScriptingParticleStamFluids::set_n, DATA_PERSISTENT | DATA_SETTABLE);

	*archive << ARCHIVE_PLUG(_turbulence, DATA_PERSISTENT | DATA_SETTABLE);
    
	*archive << ARCHIVE_DATA(_rectangle, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_diff, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_visc, DATA_PERSISTENT | DATA_SETTABLE);
	*archive << ARCHIVE_DATA(_particle_drag, DATA_PERSISTENT | DATA_SETTABLE);
    
    
	        					
    archive->pop_domain ();
}

//==============================================================================
//==============================================================================

void ScriptingParticleStamFluids::tick (const DTfloat dt)
{
	PROFILER(PARTICLES);

    // Make sure there are input particles
    std::shared_ptr<Particles> particles = _in;
    if (!particles || particles->translations_stream().size() <= 0) {
        _out.set_clean();
        return;
    }
        
    // Build the velocities stream
    if (particles->velocity_stream().size() <= 0) {
        particles->build_velocity_stream();
    }
    
    _turbulence_timer += std::abs(_turbulence->x);
    
    DTint i, j;
    FOR_EACH_CELL
        DTint index = IX(i,j);
        
        DTfloat x = Perlin::noise_3D (i / static_cast<DTfloat>(_n) * 2.0F + _turbulence_timer, j / static_cast<DTfloat>(_n) * 2.0F + _turbulence_timer, _turbulence_timer);
        DTfloat y = Perlin::noise_3D (i / static_cast<DTfloat>(_n) * 2.0F - _turbulence_timer, j / static_cast<DTfloat>(_n) * 2.0F - _turbulence_timer, -_turbulence_timer);
        
        _u_prev[index] += x * _turbulence->y;
        _v_prev[index] += y * _turbulence->y;
    END_FOR
    
	vel_step (&_u[0], &_v[0], &_u_prev[0], &_v_prev[0], _visc, dt );
	dens_step (&_dens[0], &_dens_prev[0], &_u[0], &_v[0], _diff, dt );
    
    // Do processing
    std::vector<Vector3> &velocities = particles->velocity_stream();
    std::vector<Vector3> &translations = particles->translations_stream();

    for (DTint k = particles->active_start(); k != particles->active_end(); k = (k + 1) % particles->translations_stream().size()) {
        Vector3 &translation = translations[k];
        Vector3 &velocity = velocities[k];

        DTint ix = (DTint) (_n * (translation.x - _rectangle.minus_x()) / (_rectangle.plus_x() - _rectangle.minus_x()));
        if (ix < 0)         ix = 0;
        else if (ix >= _n)  ix = _n-1;

        DTint iy = (DTint) (_n * (translation.y - _rectangle.minus_y()) / (_rectangle.plus_y() - _rectangle.minus_y()));
        if (iy < 0)         iy = 0;
        else if (iy >= _n)  iy = _n-1;
        
        DTint index = IX(ix,iy);
        
        velocity.x = _particle_drag * _u[index] * (_rectangle.plus_x() - _rectangle.minus_x()) + (1.0F-_particle_drag) * velocity.x;
        velocity.y = _particle_drag * _v[index] * (_rectangle.plus_y() - _rectangle.minus_y()) + (1.0F-_particle_drag) * velocity.y;
        velocity.z = 0.0F;

    }

    _out = particles;
}

//==============================================================================
//==============================================================================

void ScriptingParticleStamFluids::set_n (const DTint n)
{
    _n = n;
	DTint size = (_n+2)*(_n+2);

    _u.resize(size, 0.0F);
    _v.resize(size, 0.0F);
    _u_prev.resize(size, 0.0F);
    _v_prev.resize(size, 0.0F);
    _dens.resize(size, 0.0F);
    _dens_prev.resize(size, 0.0F);
}

DTint ScriptingParticleStamFluids::n (void) const
{
    return _n;
}

//==============================================================================
//==============================================================================

void ScriptingParticleStamFluids::add_source (DTfloat *x, DTfloat *s, DTfloat dt )
{
	DTint i, size=(_n+2)*(_n+2);
	for ( i=0 ; i<size ; i++ ) 
        x[i] += dt*s[i];
}

void ScriptingParticleStamFluids::set_bnd (DTint b, DTfloat* x )
{
	DTint i;

	for ( i=1 ; i<=_n ; i++ ) {
		x[IX(0  ,i)] = b==1 ? -x[IX(1,i)] : x[IX(1,i)];
		x[IX(_n+1,i)] = b==1 ? -x[IX(_n,i)] : x[IX(_n,i)];
		x[IX(i,0  )] = b==2 ? -x[IX(i,1)] : x[IX(i,1)];
		x[IX(i,_n+1)] = b==2 ? -x[IX(i,_n)] : x[IX(i,_n)];
	}
	x[IX(0  ,0  )] = 0.5f*(x[IX(1,0  )]+x[IX(0  ,1)]);
	x[IX(0  ,_n+1)] = 0.5f*(x[IX(1,_n+1)]+x[IX(0  ,_n)]);
	x[IX(_n+1,0  )] = 0.5f*(x[IX(_n,0  )]+x[IX(_n+1,1)]);
	x[IX(_n+1,_n+1)] = 0.5f*(x[IX(_n,_n+1)]+x[IX(_n+1,_n)]);
}

void ScriptingParticleStamFluids::lin_solve (DTint b, DTfloat *x, DTfloat *x0, DTfloat a, DTfloat c )
{
	DTint i, j, k;

	for ( k=0 ; k<20 ; k++ ) {
		FOR_EACH_CELL
			x[IX(i,j)] = (x0[IX(i,j)] + a*(x[IX(i-1,j)]+x[IX(i+1,j)]+x[IX(i,j-1)]+x[IX(i,j+1)]))/c;
		END_FOR
		set_bnd (b, x );
	}
}

void ScriptingParticleStamFluids::diffuse (DTint b, DTfloat *x, DTfloat *x0, DTfloat diff, DTfloat dt )
{
	DTfloat a=dt*diff*_n*_n;
	lin_solve (b, x, x0, a, 1+4*a );
}

void ScriptingParticleStamFluids::advect (DTint b, DTfloat *d, DTfloat *d0, DTfloat *u, DTfloat *v, DTfloat dt )
{
	DTint i, j, i0, j0, i1, j1;
	DTfloat x, y, s0, t0, s1, t1, dt0;

	dt0 = dt*_n;
	FOR_EACH_CELL
		x = i-dt0*u[IX(i,j)]; y = j-dt0*v[IX(i,j)];
		if (x<0.5f) x=0.5f; if (x>_n+0.5f) x=_n+0.5f; i0=(DTint)x; i1=i0+1;
		if (y<0.5f) y=0.5f; if (y>_n+0.5f) y=_n+0.5f; j0=(DTint)y; j1=j0+1;
		s1 = x-i0; s0 = 1-s1; t1 = y-j0; t0 = 1-t1;
		d[IX(i,j)] = s0*(t0*d0[IX(i0,j0)]+t1*d0[IX(i0,j1)])+
					 s1*(t0*d0[IX(i1,j0)]+t1*d0[IX(i1,j1)]);
	END_FOR
	set_bnd (b, d );
}

void ScriptingParticleStamFluids::project (DTfloat *u, DTfloat *v, DTfloat *p, DTfloat *div )
{
	DTint i, j;

	FOR_EACH_CELL
		div[IX(i,j)] = -0.5f*(u[IX(i+1,j)]-u[IX(i-1,j)]+v[IX(i,j+1)]-v[IX(i,j-1)])/_n;
		p[IX(i,j)] = 0;
	END_FOR	
	set_bnd (0, div );
    set_bnd (0, p );

	lin_solve (0, p, div, 1, 4 );

	FOR_EACH_CELL
		u[IX(i,j)] -= 0.5f*_n*(p[IX(i+1,j)]-p[IX(i-1,j)]);
		v[IX(i,j)] -= 0.5f*_n*(p[IX(i,j+1)]-p[IX(i,j-1)]);
	END_FOR
    
	set_bnd (1, u );
    set_bnd (2, v );
}

void ScriptingParticleStamFluids::dens_step (DTfloat *x, DTfloat *x0, DTfloat *u, DTfloat *v, DTfloat diff, DTfloat dt )
{
	add_source (x, x0, dt );
	SWAP ( x0, x ); 
    diffuse (0, x, x0, diff, dt );
	SWAP ( x0, x ); 
    advect (0, x, x0, u, v, dt );
}

void ScriptingParticleStamFluids::vel_step (DTfloat *u, DTfloat *v, DTfloat *u0, DTfloat *v0, DTfloat visc, DTfloat dt )
{
	add_source (u, u0, dt );
    add_source (v, v0, dt );
	SWAP ( u0, u ); 
    diffuse (1, u, u0, visc, dt );
	SWAP ( v0, v ); 
    diffuse (2, v, v0, visc, dt );
	project (u, v, u0, v0 );
	SWAP ( u0, u ); 
    SWAP ( v0, v );
	advect (1, u, u0, u0, v0, dt ); 
    advect (2, v, v0, u0, v0, dt );
	project (u, v, u0, v0 );
}



//==============================================================================
//==============================================================================

DTboolean ScriptingParticleStamFluids::compute (const PlugBase *plug)
{
	PROFILER(PARTICLES);

    if (super_type::compute(plug))  return true;

	if (plug == &_out) {
		
		_out = _in;
		_out.set_clean();
		
		return true;
	}
	
	return false;
}

//==============================================================================
//==============================================================================

void ScriptingParticleStamFluids::add_to_world(World *world)
{
    ScriptingBase::add_to_world(world);
    
    world->register_for_tick(this, make_callback(this, &type::tick));
}

void ScriptingParticleStamFluids::remove_from_world (void)
{
    world()->unregister_for_tick(this, make_callback(this, &type::tick));

    ScriptingBase::remove_from_world();
}

//==============================================================================
//==============================================================================

} // DT3

