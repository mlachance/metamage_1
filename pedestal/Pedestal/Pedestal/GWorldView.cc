/*	=============
 *	GWorldView.cc
 *	=============
 */

#include "Pedestal/GWorldView.hh"

// Nucleus
#include "Nucleus/Saved.h"


namespace Pedestal
{
	
	namespace NN = Nucleus;
	
	GWorldView::GWorldView( const Rect& bounds, Initializer )
	:
		itsBounds( bounds ),
	  	itsGWorld( N::NewGWorld( 0, N::LocalToGlobal( bounds ) ) )
	{
		Erase( bounds );
	}
	
	void GWorldView::Erase( const Rect& bounds )
	{
		NN::Saved< N::GWorld_Value > savedGWorld;
		
		N::SetGWorld( itsGWorld );
		
		N::EraseRect( bounds );
	}
	
	void GWorldView::Resize( short width, short height )
	{
		itsBounds.right = itsBounds.left + width;
		itsBounds.bottom = itsBounds.top + height;
		
		N::UpdateGWorld( itsGWorld, 0, itsBounds );
		
		Erase( itsBounds );
	}
	
	void GWorldView::Update()
	{
		// Lock pixels
		PixMapHandle pix = N::GetGWorldPixMap( itsGWorld );
		NN::Saved< N::PixelsState_Value > savedPixelsState( pix );
		
		N::LockPixels( pix );
		
		// Copy to dest
		N::CopyBits( N::GetPortBitMapForCopyBits( itsGWorld                 ),
		             N::GetPortBitMapForCopyBits( N::GetQDGlobalsThePort() ),
		             N::GetPortBounds( itsGWorld ),
		             itsBounds,
		             N::srcCopy );
		
		// Unlock pixels
	}
	
}

