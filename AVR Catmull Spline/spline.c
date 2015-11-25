/*
 * spline.c
 *
 * Created: 4/7/2015 2:56:32 PM
 *  Author: Chiuyin
 */ 

#include "spline.h"
#include <math.h>

float* _x;
float* _y;
float* _m;
int _degree;
int _length;
int _prev_point;

void SplineInit( float x[], float y[], int numPoints, int degree ) {
	_x = x;
	_y = y;
	_length = numPoints;
	_degree = degree;
}

float SplineValue( float x )
{
	if( _x[0] > x ) {
		return _y[0];
	}
	else if ( _x[_length-1] < x ) {
		return _y[_length-1];
	}
	else {
		for(int i = 0; i < _length; i++ )
		{
			int index = ( i + _prev_point ) % _length;
			
			if( _x[index] == x ) {
				_prev_point = index;
				return _y[index];
				} else if( (_x[index] < x) && (x < _x[index+1]) ) {
				_prev_point = index;
				return SplineCalc( x, index );
			}
		}
	}
}

float SplineCalc( float x, int i )
{
	switch( _degree ) {
		case 0:
		return _y[i];
		case 1:
		if( _x[i] == _x[i+1] ) {
			// Avoids division by 0
			return _y[i];
			} else {
			return _y[i] + (_y[i+1] - _y[i]) * ( x - _x[i]) / ( _x[i+1] - _x[i] );
		}
		case Catmull:
		if( i == 0 ) {
			// x prior to spline start - first point used to determine tangent
			return _y[1];
			} else if( i == _length-2 ) {
			// x after spline end - last point used to determine tangent
			return _y[_length-2];
			} else {
			float t = (x-_x[i]) / (_x[i+1]-_x[i]);
			float m0 = (i==0 ? 0 : SplineCatmull_tangent(i) );
			float m1 = (i==_length-1 ? 0 : SplineCatmull_tangent(i+1) );
			return SplineHermite( t, _y[i], _y[i+1], m0, m1, _x[i], _x[i+1]);
		}
	}
}

float SplineHermite( float t, float p0, float p1, float m0, float m1, float x0, float x1 ) {
	return (SplineHermite_00(t)*p0) + (SplineHermite_10(t)*(x1-x0)*m0) + (SplineHermite_01(t)*p1) + (SplineHermite_11(t)*(x1-x0)*m1);
}
float SplineHermite_00( float t ) { return (2*pow(t,3)) - (3*pow(t,2)) + 1;}
float SplineHermite_10( float t ) { return pow(t,3) - (2*pow(t,2)) + t; }
float SplineHermite_01( float t ) { return (3*pow(t,2)) - (2*pow(t,3)); }
float SplineHermite_11( float t ) { return pow(t,3) - pow(t,2); }

float SplineCatmull_tangent( int i )
{
	if( _x[i+1] == _x[i-1] ) {
		// Avoids division by 0
		return 0;
		} else {
		return (_y[i+1] - _y[i-1]) / (_x[i+1] - _x[i-1]);
	}
}
