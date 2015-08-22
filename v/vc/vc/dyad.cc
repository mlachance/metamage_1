/*
	dyad.cc
	-------
*/

#include "vc/dyad.hh"

// iota
#include "iota/swap.hh"


namespace vc
{
	
	void dyad::swap( dyad& b )
	{
		using iota::swap;
		
		swap( i,  b.i  );
		swap( op, b.op );
	}
	
}
