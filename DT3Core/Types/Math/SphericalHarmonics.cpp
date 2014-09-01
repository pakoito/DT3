//==============================================================================
///	
///	File: SphericalHarmonics.cpp
///	
/// Copyright (C) 2000-2014 by Smells Like Donkey Software Inc. All rights reserved.
///
/// This file is subject to the terms and conditions defined in
/// file 'LICENSE.txt', which is part of this source code package.
///	
//==============================================================================
/// Refer to the Sony paper:
/// http://www.research.scea.com/gdc2003/spherical-harmonic-lighting.pdf
//==============================================================================

#include "DT3Core/Types/Math/SphericalHarmonics.hpp"
#include "DT3Core/Types/Math/MoreMath.hpp"
#include <cmath>

//==============================================================================
//==============================================================================

namespace DT3 {

//==============================================================================
/// Standard class constructors/destructors
//==============================================================================

SphericalHarmonics::SphericalHarmonics (void)
	:	_num_bands			(0),
		_num_samples		(0),
		_num_samples_sqrt	(0)
{

}
	
SphericalHarmonics::~SphericalHarmonics (void)
{
	clear_spherical_samples ();
	clear_coefficients ();
}

//==============================================================================
/// The process for evaluating legendre polynomials turns out to be
/// quite involved, which is why they’re rarely used for
/// approximating 1D functions. The usual mathematical definition of
/// the series is defined in terms of derivatives of imaginary numbers
/// and requires a series of nasty cancellations of values that alternate
/// in sign and this is not a DTfloating point friendly process. Instead we
/// turn to a set of recurrence relations (i.e. a recursive definition) that
/// generate the current polynomial from earlier results in the series.
/// 
/// There are only three rules we need:
/// 
/// 1. (l-m)P[l]^m = x(2l-1)P[l-1]^m - (1+m-1)P[l-2]^m
/// 
/// The main term of the recurrence takes the two previous bands l–1
/// and l–2 and generates a new higher band l from them.
/// 
/// 2. P[m]^m = (-1)^m(2m-1)!!(1-x^2)^(m/2)
/// 
/// The expression is the best place to start from as it is the only rule
/// that needs no previous values. Note that x!! is the DTfloat factorial
/// function which, as (2m–1) is always odd, returns the product of all
/// odd integers less than or equal to x. We can use P[0]^0(x) = 1 as the
/// initial state for an iterative loop that hoists us up from 0 to m.
/// 
/// 3. P[m+1]^m = x(2m+1)P[m]^m
/// 
/// This expression allows us to lift a term to a higher band.
//==============================================================================

DTfloat SphericalHarmonics::legendre(DTint l,DTint m, DTfloat x)
{
	// evaluate an Associated legendre Polynomial legendre(l,m,x) at x
	DTfloat pmm = 1.0F;
	if(m>0) {
		DTfloat somx2 = std::sqrt((1.0F-x)*(1.0F+x));
		DTfloat fact = 1.0F;
		for(DTint i=1; i<=m; i++) {
			pmm *= (-fact) * somx2;
			fact += 2.0F;
		}
	}
	if(l==m) return pmm;
	
	DTfloat pmmp1 = x * (2.0F*m+1.0F) * pmm;
	if(l==m+1) return pmmp1;
	
	DTfloat pll = 0.0F;
	for(DTint ll=m+2; ll<=l; ++ll) {
		pll = ( (2.0F*ll-1.0F)*x*pmmp1-(ll+m-1.0F)*pmm ) / (ll-m);
		pmm = pmmp1;
		pmmp1 = pll;
	}
	return pll;
}


//==============================================================================
/// This is just a scaling factor to normalize the functions
//==============================================================================

DTfloat SphericalHarmonics::legendre_scale (DTint l, DTint m)
{
	// renormalisation constant for sH function
	DTfloat temp = ((2.0F * l + 1.0F) * MoreMath::factorial(l-m)) / (4.0F * PI * MoreMath::factorial(l+m));
	return std::sqrt(temp);
}

//==============================================================================
/// The code for evaluating an sH function
/// 
/// In order to generate all the sH functions, the parameters l and m
/// are defined slightly differently from the legendre polynomials – l
/// is still a positive integer starting from 0, but m takes signed integer
/// values from –l to l.
/// 
/// y[l]^m(theta,phi) -l <= m <= l
/// 
/// Sometimes it is useful to think of the sH functions occurring in a
/// specific order so that we can flatten them into a 1D vector, so we
/// will also define the sequence yi
/// 
/// y[l]^m(theta,phi) = y[i](theta,phi) where i = l (l+1)+m
/// 
//==============================================================================

DTfloat SphericalHarmonics::sH(DTint l, DTint m, DTfloat theta, DTfloat phi)
{
	// Return a point sample of a Spherical Harmonic basis function
	// l is the band, range [0..N]
	// m in the range [-l..l]
	// theta in the range [0..Pi]
	// phi in the range [0..2*Pi]
	static const DTfloat sqrt2 = std::sqrt(2.0F);
	
	if(m==0) return legendre_scale(l,0) * legendre(l,m, std::cos(theta));
	else if(m>0) return sqrt2 * legendre_scale(l,m) * std::cos(m*phi) * legendre(l,m,std::cos(theta));
	else return sqrt2 * legendre_scale(l,-m) * std::sin(-m*phi) * legendre(l,-m,std::cos(theta));
}

//==============================================================================
/// Main routines for setting up the sH coefficients. Takes an uninitialized
/// 2D array and the size of one edge of the 2D array.
//==============================================================================

void SphericalHarmonics::build_spherical_samples(DTint num_bands, DTint num_samples)
{
	
	clear_spherical_samples();
	
	_num_bands = num_bands;
	_num_samples_sqrt = (DTint) std::sqrt(static_cast<DTfloat>(num_samples));
	_num_samples = _num_samples_sqrt * _num_samples_sqrt;

	// allocate Samples
	_samples.resize(_num_samples);
	for (DTint k = 0; k < _num_samples; ++k) {
		_samples[k]._sph.clear();
		_samples[k]._vec.clear();
		_samples[k]._coeff.resize(num_coefficients());
		
		DTint num_coeff = num_coefficients();
		for (DTint j = 0; j < num_coeff; ++j)
			_samples[k]._coeff[j] = 0.0F;
	}
	
	// fill an N*N*2 array with uniformly distributed
	// _samples across the sphere using jittered stratification
	
	DTint i=0; // array index
	DTfloat oneoverN = 1.0F/_num_samples_sqrt;
	
	for(DTint a = 0; a < _num_samples_sqrt; a++) {
		for(DTint b = 0; b < _num_samples_sqrt; b++) {
			
			// generate unbiased distribution of spherical coords
			DTfloat x = (a + MoreMath::random_float()) * oneoverN; // do not reuse results
			DTfloat y = (b + MoreMath::random_float()) * oneoverN; // each sample must be random
			
			// Mapping [0..1,0..1] random numbers into spherical coordinates
			DTfloat theta = 2.0F * std::acos(std::sqrt(1.0F - x));
			DTfloat phi = TWO_PI * y;
			
			// Angle representation
			ASSERT(i>= 0 && i < _num_samples);
			_samples[i]._sph = Vector3(theta,phi,1.0F);
			
			// Unit vector representation
			Vector3 vec(std::sin(theta) * std::cos(phi), std::sin(theta) * std::sin(phi), std::cos(theta));
			_samples[i]._vec = vec;
			
			// precompute all sH coefficients for this sample
			for(DTint l=0; l<_num_bands; ++l) {
				for(DTint m=-l; m<=l; ++m) {
					DTint index = l*(l+1)+m;
					
					ASSERT(index>= 0 && index < num_coefficients());
					_samples[i]._coeff[index] = sH(l,m,theta,phi);
				}
			}
			
			++i;
			
		} // a
	} // b
}

//==============================================================================
/// Builds the coefficients for a function
//==============================================================================

void SphericalHarmonics::build_coefficients(Polar_Function fn)
{
	// allocate results
	clear_coefficients();
	_coeff.resize(num_coefficients());

	const DTfloat weight = 4.0F * PI;
	// for each sample
	for(DTint i = 0; i < _num_samples; ++i) {
		DTfloat theta = _samples[i]._sph.x;
		DTfloat phi = _samples[i]._sph.y;
		
		DTint num_coeff = num_coefficients();
		for(DTint n = 0; n < num_coeff; ++n) {
			_coeff[n] += fn(theta,phi) * _samples[i]._coeff[n];
		}
	}
	
	// divide the _coeff by weight and number of samples
	DTfloat factor = weight / _num_samples;
	DTint num_coeff = num_coefficients();
	for(DTint i = 0; i < num_coeff; ++i) {
		_coeff[i] = _coeff[i] * factor;
	}
}

//==============================================================================
/// Main routines for setting up the sH coefficients. Takes an uninitialized
/// 2D array and the size of one edge of the 2D array.
//==============================================================================
		
void SphericalHarmonics::clear_spherical_samples (void)
{
    _samples.resize(0);
}

//==============================================================================
//==============================================================================

void SphericalHarmonics::clear_coefficients		(void)
{
    _coeff.resize(0);
}

//==============================================================================
//==============================================================================

} // DT3
