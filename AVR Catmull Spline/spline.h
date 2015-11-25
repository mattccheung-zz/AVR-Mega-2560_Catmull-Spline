/*
 * spline.h
 *
 * Created: 4/7/2015 2:56:24 PM
 *  Author: Chiuyin
 */ 

/*
  Library for 1-d splines
  Copyright Ryan Michael
  Licensed under the LGPLv3 
*/
#ifndef spline_h
#define spline_h


#define Hermite 10
#define Catmull 11


    void SplineInit( float x[], float y[], int numPoints, int degree );
    float SplineValue( float x );
    float SplineCalc( float x, int i );
    

    
    float SplineHermite( float t, float p0, float p1, float m0, float m1, float x0, float x1 );
    float SplineHermite_00( float t );
    float SplineHermite_10( float t );
	float SplineHermite_01( float t );
	float SplineHermite_11( float t );
    float SplineCatmull_tangent( int i );


#endif
