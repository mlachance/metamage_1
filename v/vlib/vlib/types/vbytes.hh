/*
	vbytes.hh
	---------
*/

#ifndef VLIB_TYPES_VBYTES_HH
#define VLIB_TYPES_VBYTES_HH

// vlib
#include "vlib/value.hh"


namespace vlib
{
	
	struct dispatch;
	struct stringify;
	
	extern const stringify vbytes_cpy;
	
	class VBytes : public Value
	{
		public:
			VBytes( const plus::string& s, value_type type, const dispatch* d )
			:
				Value( (const vu_string&) s, type, d )
			{
			}
	};
	
}

#endif
