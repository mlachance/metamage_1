// pump.hh
// -------
//
// Maintained by Joshua Juran

// Part of the Nitrogen project.
//
// Written 2007-2008 by Joshua Juran.
//
// This code was written entirely by the above contributor, who places it
// in the public domain.


#ifndef POSEVEN_EXTRAS_PUMP_HH
#define POSEVEN_EXTRAS_PUMP_HH

// Relix
#include "relix/pump.h"

// poseven
#ifndef POSEVEN_TYPES_ERRNO_T_HH
#include "poseven/types/errno_t.hh"
#endif
#ifndef POSEVEN_TYPES_FD_T_HH
#include "poseven/types/fd_t.hh"
#endif


namespace poseven
{
	
	inline ssize_t pump( fd_t    fd_in,
	                     off_t*  off_in,
	                     fd_t    fd_out,
	                     off_t*  off_out = NULL,
	                     size_t  count   = 0 )
	{
		return throw_posix_result( ::pump( fd_in, off_in, fd_out, off_out, count, 0 ) );
	}
	
	inline ssize_t pump( fd_t    fd_in,
	                     fd_t    fd_out,
	                     off_t*  off_out = NULL,
	                     size_t  count   = 0 )
	{
		return pump( fd_in, NULL, fd_out, off_out, count );
	}
	
}

#endif
