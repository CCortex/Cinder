/*
 Copyright (c) 2010, The Barbarian Group
 All rights reserved.

 Redistribution and use in source and binary forms, with or without modification, are permitted provided that
 the following conditions are met:

    * Redistributions of source code must retain the above copyright notice, this list of conditions and
	the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice, this list of conditions and
	the following disclaimer in the documentation and/or other materials provided with the distribution.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED
 WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
 PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
 ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
 TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#include "cinder/Shape2d.h"

using std::vector;

namespace cinder {

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void Shape2d::moveTo( const vec2 &p )
{
	mContours.push_back( Path2d() );
	mContours.back().moveTo( p );
}

void Shape2d::lineTo( const vec2 &p )
{
	mContours.back().lineTo( p );
}

void Shape2d::quadTo( const vec2 &p1, const vec2 &p2 )
{
	mContours.back().quadTo( p1, p2 );
}

void Shape2d::curveTo( const vec2 &p1, const vec2 &p2, const vec2 &p3 )
{
	mContours.back().curveTo( p1, p2, p3 );
}

void Shape2d::arc( const vec2 &center, float radius, float startRadians, float endRadians, bool forward )
{
	if( mContours.empty() )
		mContours.push_back( Path2d() );
	mContours.back().arc( center, radius, startRadians, endRadians, forward );
}

void Shape2d::arcTo( const vec2 &p, const vec2 &t, float radius )
{
	mContours.back().arcTo( p, t, radius );
}

void Shape2d::close()
{
	mContours.back().close();
}

void Shape2d::append( const Shape2d &shape )
{
	for( vector<Path2d>::const_iterator pathIt = shape.getContours().begin(); pathIt != shape.getContours().end(); ++pathIt )
		appendContour( *pathIt );
}

void Shape2d::scale( const vec2 &amount, vec2 scaleCenter )
{
	for( vector<Path2d>::iterator contIt = mContours.begin(); contIt != mContours.end(); ++contIt )
		contIt->scale( amount, scaleCenter );
}

void Shape2d::transform( const mat3 &matrix )
{
	for( vector<Path2d>::iterator contIt = mContours.begin(); contIt != mContours.end(); ++contIt )
		contIt->transform( matrix );
}

Shape2d	Shape2d::transformed( const mat3 &matrix ) const
{
	Shape2d result;
	for( vector<Path2d>::const_iterator contIt = mContours.begin(); contIt != mContours.end(); ++contIt )
		result.appendContour( contIt->transformed( matrix ) );
	return result;
}

Rectf Shape2d::calcBoundingBox() const
{
	auto result = Rectf( vec2(), vec2() );

	// find the first point and initialize the result with that
	for( vector<Path2d>::const_iterator contIt = mContours.begin(); contIt != mContours.end(); ++contIt ) {
		if( ! contIt->mPoints.empty() ) {
			result = Rectf( contIt->mPoints[0], contIt->mPoints[0] );
			break;
		}
	}

	// now iterate all the contours and expand the result to include their points
	for( vector<Path2d>::const_iterator contIt = mContours.begin(); contIt != mContours.end(); ++contIt ) {
		result.include( contIt->mPoints );
	}
	
	return result;
}

Rectf Shape2d::calcPreciseBoundingBox() const
{
	auto result = Rectf( vec2(), vec2() );

	// find the first point and initialize the result with that
	for( vector<Path2d>::const_iterator contIt = mContours.begin(); contIt != mContours.end(); ++contIt ) {
		if( ! contIt->mPoints.empty() ) {
			result = Rectf( contIt->mPoints[0], contIt->mPoints[0] );
			break;
		}
	}

	// now iterate all the contours and expand the result to include their points
	for( vector<Path2d>::const_iterator contIt = mContours.begin(); contIt != mContours.end(); ++contIt ) {
		result.include( contIt->calcPreciseBoundingBox() );
	}
	
	return result;
}

bool Shape2d::contains( const vec2 &pt ) const
{
	int numPathsInside = 0;
	for( vector<Path2d>::const_iterator contIt = mContours.begin(); contIt != mContours.end(); ++contIt ) {
		if( contIt->contains( pt ) )
			numPathsInside++;
	}
	
	return ( numPathsInside % 2 ) == 1;
}

} // namespace cinder
