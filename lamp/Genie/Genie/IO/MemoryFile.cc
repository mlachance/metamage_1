/*	=============
 *	MemoryFile.cc
 *	=============
 */

#include "Genie/IO/MemoryFile.hh"

// Nucleus
#include "Nucleus/NAssert.h"

// Io
#include "io/io.hh"


namespace Genie
{
	
	MemoryFileHandle::~MemoryFileHandle()
	{
	}
	
	int MemoryFileHandle::SysRead( char* data, std::size_t byteCount )
	{
		ASSERT( GetFileMark() <= itsSize );
		
		byteCount = std::min( byteCount, itsSize - GetFileMark() );
		
		std::copy( itsBase + GetFileMark(),
		           itsBase + GetFileMark() + byteCount,
		           data );
		
		return Advance( byteCount );
	}
	
	int MemoryFileHandle::SysWrite( const char* data, std::size_t byteCount )
	{
		std::copy( data,
		           data + byteCount,
		           itsBase + GetFileMark() );
		
		return Advance( byteCount );
	}
	
}

