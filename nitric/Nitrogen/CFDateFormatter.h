// Nitrogen/CFDateFormatter.h
// --------------------------

// Part of the Nitrogen project.
//
// Written 2004-2007 by Marshall Clow and Joshua Juran.
//
// This code was written entirely by the above contributors, who place it
// in the public domain.


#ifndef NITROGEN_CFDATEFORMATTER_H
#define NITROGEN_CFDATEFORMATTER_H

#if	!TARGET_RT_MAC_MACHO
#error "These routines are only directly callable from MachO"
#endif

#ifndef __COREFOUNDATION_CFDATEFORMATTER__
#include <CoreFoundation/CFDateFormatter.h>
#endif

#ifndef NITROGEN_CFBASE_H
#include "Nitrogen/CFBase.h"
#endif
#ifndef NITROGEN_CFDATE_H
#include "Nitrogen/CFDate.h"	// We need this for the defs for Owned < CFDateRef >
#endif

#ifndef NUCLEUS_OWNED_H
#include "Nucleus/Owned.h"
#endif

namespace Nitrogen {
	using ::CFDateFormatterRef;
   }

namespace Nucleus
   {
	template <> struct Disposer_Traits< Nitrogen::CFDateFormatterRef >: Disposer_Traits< Nitrogen::CFTypeRef >  {};
  }

namespace Nitrogen
  {
	template <> struct CFType_Traits< CFDateFormatterRef >: Basic_CFType_Traits< CFDateFormatterRef, ::CFDateFormatterGetTypeID > {};

	inline Nucleus::Owned<CFDateFormatterRef> CFDateFormatterCreate ( CFAllocatorRef allocator, 
			CFLocaleRef locale, CFDateFormatterStyle dateStyle, CFDateFormatterStyle timeStyle ) {
		return Nucleus::Owned<CFDateFormatterRef>::Seize ( ::CFDateFormatterCreate ( allocator,
				locale, dateStyle, timeStyle ));
		}

	
//	CFLocaleRef CFDateFormatterGetLocale ( CFDateFormatterRef formatter );
	using ::CFDateFormatterGetLocale;

//	CFDateFormatterStyle CFDateFormatterGetDateStyle ( CFDateFormatterRef formatter );
	using ::CFDateFormatterGetDateStyle;

//	CFDateFormatterStyle CFDateFormatterGetTimeStyle ( CFDateFormatterRef formatter );
	using ::CFDateFormatterGetTimeStyle;
	
//	CFStringRef CFDateFormatterGetFormat ( CFDateFormatterRef formatter );
	using ::CFDateFormatterGetFormat;

//	void CFDateFormatterSetFormat ( CFDateFormatterRef formatter, CFStringRef formatString );
	using ::CFDateFormatterSetFormat;
	
	inline Nucleus::Owned<CFStringRef> CFDateFormatterCreateStringWithDate ( CFAllocatorRef allocator, CFDateFormatterRef formatter, CFDateRef date ) {
		return Nucleus::Owned<CFStringRef>::Seize ( ::CFDateFormatterCreateStringWithDate ( allocator, formatter, date ));
		}
		

	inline Nucleus::Owned<CFStringRef> CFDateFormatterCreateStringWithAbsoluteTime ( CFAllocatorRef allocator, CFDateFormatterRef formatter, CFAbsoluteTime at ) {
		return Nucleus::Owned<CFStringRef>::Seize ( ::CFDateFormatterCreateStringWithAbsoluteTime ( allocator, formatter, at ));
		}

	inline Nucleus::Owned<CFDateRef> CFDateFormatterCreateDateFromString ( CFAllocatorRef allocator, CFDateFormatterRef formatter, CFStringRef string, CFRange *rangep = NULL ) {
		return Nucleus::Owned<CFDateRef>::Seize ( ::CFDateFormatterCreateDateFromString ( allocator, formatter, string, rangep ));
		}
		
	struct CFDateFormatterGetAbsoluteTimeFromString_Failed {};
	inline CFAbsoluteTime CFDateFormatterGetAbsoluteTimeFromString ( CFDateFormatterRef formatter, CFStringRef string, CFRange *rangep = NULL ) {
		CFAbsoluteTime result;
		if ( !::CFDateFormatterGetAbsoluteTimeFromString ( formatter, string, rangep, &result ))
			throw CFDateFormatterGetAbsoluteTimeFromString_Failed ();
		return result;
		}
	

//	void CFDateFormatterSetProperty ( CFDateFormatterRef formatter, CFStringRef key, CFTypeRef value );
	using ::CFDateFormatterSetProperty;
	
	inline Nucleus::Owned<CFTypeRef> CFDateFormatterCopyProperty ( CFDateFormatterRef formatter, CFStringRef key ) {
		return Nucleus::Owned<CFTypeRef>::Seize ( ::CFDateFormatterCopyProperty ( formatter, key ));
		}
	
	}

#endif

