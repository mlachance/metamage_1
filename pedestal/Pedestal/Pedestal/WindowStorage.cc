/*
	WindowStorage.cc
	----------------
*/

#include "Pedestal/WindowStorage.hh"

// Mac OS
#ifndef __MACWINDOWS__
#include <MacWindows.h>
#endif

// MacGlue
#include "MacGlue/MacGlue.hh"

// Nitrogen
#include "Mac/Toolbox/Utilities/ThrowOSStatus.hh"

// MacFeatures
#include "MacFeatures/ColorQuickdraw.hh"

// Pedestal
#include "Pedestal/View.hh"


namespace MacGlue
{
	
	DECLARE_MAC_GLUE( NewWindow  );
	DECLARE_MAC_GLUE( NewCWindow );
	
}

namespace Pedestal
{
	
	namespace n = nucleus;
	
	enum
	{
		kWindowCreator = ':-)\xCA',  // Yes, I actually registered this
		
		kWindowOwnerTag = 'This',  // Address of owning object
		kWindowViewTag  = 'View',  // Address of view object, if any
		
		kWindowClosedProcTag  = 'Clsd',
		kWindowResizedProcTag = 'Rszd',
	};
	
#if ! OPAQUE_TOOLBOX_STRUCTS
	
	struct WindowStorage
	{
		Window*             owner;
		View*               view;
		WindowClosed_proc   closed_proc;
		WindowResized_proc  resized_proc;
		WindowAttributes    attributes;
		
		WindowRecord        window;
		
		WindowStorage() : view(), closed_proc(), resized_proc()
		{
		}
	};
	
	static
	WindowStorage* RecoverWindowStorage( WindowRef window )
	{
		if ( GetWindowKind( window ) != kApplicationWindowKind )
		{
			return NULL;
		}
		
		void* address = (char*) window - offsetof( WindowStorage, window );
		
		return (WindowStorage*) address;
	}
	
	static
	pascal void DestroyWindow( WindowRef window )
	{
		WindowStorage* storage = RecoverWindowStorage( window );
		
		CloseWindow( window );
		
		delete storage;
	}
	
#endif
	
	WindowAttributes get_window_attributes( WindowRef window )
	{
	#if ! TARGET_API_MAC_CARBON
		
		if ( WindowStorage* storage = RecoverWindowStorage( window ) )
		{
			return storage->attributes;
		}
		
		return kWindowNoAttributes;
		
	#endif
		
		OSStatus err;
		
		WindowAttributes attrs = kWindowNoAttributes;
		err = GetWindowAttributes( window, &attrs );
		
		return attrs;
	}
	
	void set_window_owner( WindowRef window, Window* owner )
	{
	#if ! TARGET_API_MAC_CARBON
		
		WindowStorage* storage = RecoverWindowStorage( window );
		
		/*
			This is only called on windows created below, which will always
			be application windows with custom storage.
		*/
		
		storage->owner = owner;
		
		return;
		
	#endif
		
		OSStatus err;
		
		err = SetWindowProperty( window,
		                         kWindowCreator,
		                         kWindowOwnerTag,
		                         sizeof owner,
		                         &owner );
		
		Mac::ThrowOSStatus( err );
	}
	
	Window* get_window_owner( WindowRef window )
	{
	#if ! TARGET_API_MAC_CARBON
		
		if ( WindowStorage* storage = RecoverWindowStorage( window ) )
		{
			return storage->owner;
		}
		
		return NULL;
		
	#endif
		
		OSStatus err;
		
		Window* result = NULL;
		err = GetWindowProperty( window,
		                         kWindowCreator,
		                         kWindowOwnerTag,
		                         sizeof result,
		                         NULL,
		                         &result );
		
		return result;
	}
	
	void set_window_view( WindowRef window, View* view )
	{
	#if ! TARGET_API_MAC_CARBON
		
		if ( WindowStorage* storage = RecoverWindowStorage( window ) )
		{
			View* old_view = storage->view;
			
			storage->view = view;
			
			if ( view )
			{
				intrusive_ptr_add_ref( view );
			}
			
			if ( old_view )
			{
				intrusive_ptr_release( old_view );
			}
		}
		
		return;
		
	#endif
		
		OSStatus err;
		
		View* old_view = get_window_view( window );
		
		err = SetWindowProperty( window,
		                         kWindowCreator,
		                         kWindowViewTag,
		                         sizeof view,
		                         &view );
		
		Mac::ThrowOSStatus( err );
		
		if ( view )
		{
			intrusive_ptr_add_ref( view );
		}
		
		if ( old_view )
		{
			intrusive_ptr_release( old_view );
		}
	}
	
	View* get_window_view( WindowRef window )
	{
	#if ! TARGET_API_MAC_CARBON
		
		if ( WindowStorage* storage = RecoverWindowStorage( window ) )
		{
			return storage->view;
		}
		
		return NULL;
		
	#endif
		
		OSStatus err;
		
		View* result = NULL;
		err = GetWindowProperty( window,
		                         kWindowCreator,
		                         kWindowViewTag,
		                         sizeof result,
		                         NULL,
		                         &result );
		
		return result;
	}
	
	void set_window_closed_proc( WindowRef          window,
	                             WindowClosed_proc  proc )
	{
	#if ! TARGET_API_MAC_CARBON
		
		if ( WindowStorage* storage = RecoverWindowStorage( window ) )
		{
			storage->closed_proc = proc;
		}
		
		return;
		
	#endif
		
		OSStatus err;
		
		err = SetWindowProperty( window,
		                         kWindowOwnerTag,
		                         kWindowClosedProcTag,
		                         sizeof proc,
		                         &proc );
		
		Mac::ThrowOSStatus( err );
	}
	
	static
	WindowClosed_proc get_window_closed_proc( WindowRef window )
	{
	#if ! TARGET_API_MAC_CARBON
		
		if ( WindowStorage* storage = RecoverWindowStorage( window ) )
		{
			return storage->closed_proc;
		}
		
		return NULL;
		
	#endif
		
		OSStatus err;
		
		WindowClosed_proc result = NULL;
		err = GetWindowProperty( window,
		                         kWindowOwnerTag,
		                         kWindowClosedProcTag,
		                         sizeof result,
		                         NULL,
		                         &result );
		
		return result;
	}
	
	void set_window_resized_proc( WindowRef           window,
	                              WindowResized_proc  proc )
	{
	#if ! TARGET_API_MAC_CARBON
		
		if ( WindowStorage* storage = RecoverWindowStorage( window ) )
		{
			storage->resized_proc = proc;
		}
		
		return;
		
	#endif
		
		OSStatus err;
		
		err = SetWindowProperty( window,
		                         kWindowOwnerTag,
		                         kWindowResizedProcTag,
		                         sizeof proc,
		                         &proc );
		
		Mac::ThrowOSStatus( err );
	}
	
	WindowResized_proc get_window_resized_proc( WindowRef window )
	{
	#if ! TARGET_API_MAC_CARBON
		
		if ( WindowStorage* storage = RecoverWindowStorage( window ) )
		{
			return storage->resized_proc;
		}
		
		return NULL;
		
	#endif
		
		OSStatus err;
		
		WindowResized_proc result = NULL;
		err = GetWindowProperty( window,
		                         kWindowOwnerTag,
		                         kWindowResizedProcTag,
		                         sizeof result,
		                         NULL,
		                         &result );
		
		return result;
	}
	
	void close_window( WindowRef window )
	{
		if ( WindowClosed_proc proc = get_window_closed_proc( window ) )
		{
			proc( window );
		}
		else
		{
			DisposeWindow( window );
		}
	}
	
	
	typedef pascal WindowRef (*NewWindow_ProcPtr)( void*             storage,
	                                               const Rect*       bounds,
	                                               ConstStr255Param  title,
	                                               Boolean           visible,
	                                               short             procID,
	                                               WindowRef         behind,
	                                               Boolean           goAwayFlag,
	                                               long              refCon );
	
	using MacFeatures::Has_ColorQuickdraw;
	
	static const NewWindow_ProcPtr gNewWindow = Has_ColorQuickdraw() ? &MacGlue::NewCWindow
	                                                                 : &MacGlue::NewWindow;
	
	
	static inline
	bool has_grow_box( short procID )
	{
		// This works for the standard WDEF.
		
		return (procID & ~0x8) == 0;  // documentProc (0) or zoomDocProc (8)
	}
	
	static inline
	bool has_zoom_box( short procID )
	{
		// This works for the standard WDEF.
		
		return (procID & ~0x4) == 8;  // zoomDocProc (8) or zoomNoGrow (12)
	}
	
	n::owned< WindowRef > CreateWindow( const Rect&           bounds,
	                                    ConstStr255Param      title,
	                                    bool                  visible,
	                                    Mac::WindowDefProcID  procID,
	                                    bool                  goAwayFlag )
	{
		typedef nucleus::disposer_class< WindowRef >::type Disposer;
		
		Disposer disposer;
		
		WindowRef substorage = NULL;
		
	#if ! TARGET_API_MAC_CARBON
		
		disposer = &DestroyWindow;
		
		WindowStorage* storage = new WindowStorage;
		
		storage->attributes = kWindowResizableAttribute * has_grow_box( procID )
		                    | kWindowFullZoomAttribute  * has_zoom_box( procID )
		                    | kWindowCloseBoxAttribute  * goAwayFlag;
		
		substorage = &storage->window.port;
		
	#endif
		
		WindowRef window = gNewWindow( substorage,
		                               &bounds,
		                               title,
		                               visible,
		                               procID,
		                               kFirstWindowOfClass,
		                               goAwayFlag,
		                               0 );
		
		if ( window == NULL )
		{
			OSStatus err = MemError();
			
			if ( err == noErr )
			{
				err = paramErr;  // E.g. invalid procID
			}
			
		#if ! TARGET_API_MAC_CARBON
			
			delete storage;
			
		#endif
			
			Mac::ThrowOSStatus( err );
		}
		
		SetPortWindowPort( window );
		
		return n::owned< WindowRef >::seize( window, disposer );
	}
	
	n::owned< WindowRef > CreateWindow( Mac::WindowClass       wClass,
	                                    Mac::WindowAttributes  attrs,
	                                    const Rect&            bounds )
	{
		OSStatus err;
		
		WindowRef window;
		err = CreateNewWindow( wClass, attrs, &bounds, &window );
		
		Mac::ThrowOSStatus( err );
		
		SetPortWindowPort( window );
		
		typedef nucleus::disposer_class< WindowRef >::type Disposer;
		
		return n::owned< WindowRef >::seize( window, Disposer() );
	}
	
}
