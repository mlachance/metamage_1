/*	=======================
 *	FSTree_sys_mac_xpram.cc
 *	=======================
 */

#if !TARGET_API_MAC_CARBON

#include "Genie/FS/FSTree_sys_mac_xpram.hh"

// Mac OS
#include <MixedMode.h>
#include <Traps.h>


#if TARGET_CPU_68K

static asm void ReadXPRam( void* buffer, UInt16 length, UInt16 offset )
{
	MOVEA.L	4(SP),A0  ;  // load buffer address
	MOVE.L	8(SP),D0  ;  // load combined length/offset
	
	_ReadXPRam
	
	RTS
}

#else

static const short ReadXPRam_68k_code[] =
{
	0x206f, 0x0004,  // MOVEA.L 4(SP),A0
	0x202f, 0x0008,  // MOVE.L 8(SP),D0
	_ReadXPRam,
	0x4e75           // RTS
};

static const UInt32 kReadXPRamProcInfo = kCStackBased
                                       | STACK_ROUTINE_PARAMETER( 1, SIZE_CODE( sizeof (void*)     ) )
                                       | STACK_ROUTINE_PARAMETER( 2, SIZE_CODE( sizeof (short) * 2 ) );

static UniversalProcPtr gReadXPRamUPP = NewRoutineDescriptor( (ProcPtr) ReadXPRam_68k_code,
                                                              kReadXPRamProcInfo,
                                                              kM68kISA );

inline void ReadXPRam( void* buffer, UInt16 length, UInt16 offset )
{
	CallUniversalProc( gReadXPRamUPP, kReadXPRamProcInfo, buffer, (length << 16) | offset );
}

#endif

namespace Nitrogen
{
	
	inline std::string ReadXPRam( UInt16 length = 256, UInt16 offset = 0 )
	{
		std::string xpram;
		
		xpram.resize( length );
		
		::ReadXPRam( &xpram[0], length, offset );
		
		return xpram;
	}
	
}

namespace Genie
{
	
	namespace N = Nitrogen;
	
	
	std::string sys_mac_xpram_Query::Get() const
	{
		return N::ReadXPRam();
	}
	
	std::string sys_mac_xpram::Read( const FSTree* )
	{
		return N::ReadXPRam();
	}
	
}

#endif

