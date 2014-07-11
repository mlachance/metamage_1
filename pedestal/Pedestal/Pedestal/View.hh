/*	=======
 *	View.hh
 *	=======
 */

#ifndef PEDESTAL_VIEW_HH
#define PEDESTAL_VIEW_HH

// Debug
#include "debug/boost_assert.hh"

// plus
#include "plus/ref_count.hh"

// Boost
#include <boost/intrusive_ptr.hpp>

// Pedestal
#ifndef PEDESTAL_COMMANDCODE_HH
#include "Pedestal/CommandCode.hh"
#endif


struct EventRecord;      // Events.h
struct Rect;             // Quickdraw.h


namespace Pedestal
{
	
	class Quasimode;
	
	class View : public plus::ref_count< View >
	{
		public:
			virtual ~View()  {}
			
			virtual void Install  ( const Rect& bounds )  {}
			virtual void Uninstall(                    );
			
			virtual void SetBounds( const Rect& bounds )  {}
			
			virtual void Idle     ( const EventRecord& event )  {}
			virtual bool MouseDown( const EventRecord& event )  { return true;  }
			virtual bool KeyDown  ( const EventRecord& event )  { return false; }
			virtual bool HitTest  ( const EventRecord& event )  { return true;  }
			
			virtual boost::intrusive_ptr< Quasimode > EnterShiftSpaceQuasimode( const EventRecord& );
			
			virtual void Draw( const Rect& bounds, bool erasing );
			
			virtual void Activate( bool activating )  {}
			
			virtual void Focus()  {}
			virtual void Blur ()  {}
			virtual void Cue  ()  {}
			
			virtual View* AdvanceFocus( View* current, bool backward = false )
			{
				return current;
			}
			
			virtual bool SetCursor( const EventRecord& event )  { return false; }
			
			virtual bool UserCommand( CommandCode code  )  { return false; }
	};
	
}

#endif

