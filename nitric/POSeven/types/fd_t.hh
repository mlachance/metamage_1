// fd_t.hh
// -------
//
// Maintained by Joshua Juran

// Part of the Nitrogen project.
//
// Written 2008 by Joshua Juran.
//
// This code was written entirely by the above contributor, who places it
// in the public domain.


#ifndef POSEVEN_TYPES_FD_T_HH
#define POSEVEN_TYPES_FD_T_HH

// POSIX
#include <unistd.h>

// Nucleus
#include "Nucleus/Enumeration.h"


namespace poseven
{
	
	enum fd_t
	{
		stdin_fileno  = STDIN_FILENO,
		stdout_fileno = STDOUT_FILENO,
		stderr_fileno = STDERR_FILENO,
		
		fd_t_max = Nucleus::Enumeration_Traits< int >::max
	};
	
}

#endif

