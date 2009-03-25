/*	====================
 *	FSTree_new_button.cc
 *	====================
 */

#include "Genie/FileSystem/FSTree_new_button.hh"

// POSeven
#include "POSeven/Errno.hh"

// Pedestal
#include "Pedestal/TrackControl.hh"
#include "Pedestal/PushButton.hh"

// Genie
#include "Genie/FileSystem/FSTree_Directory.hh"
#include "Genie/FileSystem/FSTree_Property.hh"
#include "Genie/FileSystem/Views.hh"
#include "Genie/IO/VirtualFile.hh"


namespace Genie
{
	
	namespace N = Nitrogen;
	namespace NN = Nucleus;
	namespace p7 = poseven;
	namespace Ped = Pedestal;
	
	
	struct Button_Parameters
	{
		std::size_t  seed;
		N::Str255    title;
		bool         title_changed;
		bool         installed;
		
		Button_Parameters() : seed(), title( "\p" "OK" ), title_changed(), installed()
		{
		}
	};
	
	typedef std::map< const FSTree*, Button_Parameters > ButtonMap;
	
	static ButtonMap gButtonMap;
	
	
	struct Control_UserData : Pedestal::Control_UserData
	{
		const FSTree*  key;
		
		Control_UserData() : key()
		{
		}
	};
	
	static void DebriefAfterTrack( ControlRef control )
	{
		Control_UserData* userData = N::GetControlReference( control );
		
		ASSERT( userData      != NULL );
		ASSERT( userData->key != NULL );
		
		const FSTree* button = userData->key;
		
		ButtonMap::iterator it = gButtonMap.find( button );
		
		if ( it != gButtonMap.end() )
		{
			++it->second.seed;
		}
	}
	
	class PushButton : public Ped::PushButton
	{
		private:
			typedef const FSTree* Key;
			
			Control_UserData itsUserData;
		
		public:
			PushButton( Key key )
			{
				itsUserData.trackHook = &Ped::TrackControl;
				itsUserData.afterHook = &DebriefAfterTrack;
				itsUserData.key = key;
			}
			
			N::Str255 Title() const  { return gButtonMap[ itsUserData.key ].title; }
			
			N::RefCon RefCon() const  { return &itsUserData; }
			
			void Install( const Rect& bounds );
			
			void Uninstall();
			
			void Draw( const Rect& bounds, bool erasing );
	};
	
	void PushButton::Install( const Rect& bounds )
	{
		Ped::PushButton::Install( bounds );
		
		gButtonMap[ itsUserData.key ].installed = true;
	}
	
	void PushButton::Uninstall()
	{
		Ped::PushButton::Uninstall();
		
		gButtonMap[ itsUserData.key ].installed = false;
	}
	
	void PushButton::Draw( const Rect& bounds, bool erasing )
	{
		Ped::PushButton::Draw( bounds, erasing );
		
		Button_Parameters& params = gButtonMap[ itsUserData.key ];
		
		if ( params.title_changed )
		{
			params.title_changed = false;
			
			N::SetControlTitle( Get(), Title() );
		}
	}
	
	static boost::shared_ptr< Ped::View > CreateView( const FSTree* delegate )
	{
		return boost::shared_ptr< Ped::View >( new PushButton( delegate ) );
	}
	
	
	static void DestroyDelegate( const FSTree* delegate )
	{
		gButtonMap.erase( delegate );
	}
	
	
	struct Button_Title
	{
		static std::string Get( const FSTree* that, bool binary )
		{
			return NN::Convert< std::string >( gButtonMap[ GetViewKey( that ) ].title );
		}
		
		static void Set( const FSTree* that, const char* begin, const char* end, bool binary )
		{
			const FSTree* view = GetViewKey( that );
			
			Button_Parameters& params = gButtonMap[ view ];
			
			params.title = N::Str255( begin, end - begin );
			
			params.title_changed = true;
			
			InvalidateWindowForView( view );
		}
	};
	
	
	class Button_socket_Handle : public VirtualFileHandle
	{
		private:
			std::size_t itsSeed;
		
		public:
			Button_socket_Handle( const FSTreePtr& file, OpenFlags flags );
			
			boost::shared_ptr< IOHandle > Clone();
			
			unsigned int SysPoll();
			
			ssize_t SysRead( char* buffer, std::size_t byteCount );
	};
	
	Button_socket_Handle::Button_socket_Handle( const FSTreePtr& file, OpenFlags flags )
	:
		VirtualFileHandle( file, flags ),
		itsSeed( gButtonMap[ file->ParentRef().get() ].seed )
	{
	}
	
	boost::shared_ptr< IOHandle > Button_socket_Handle::Clone()
	{
		return boost::shared_ptr< IOHandle >( new Button_socket_Handle( GetFile(), GetFlags() ) );
	}
	
	unsigned int Button_socket_Handle::SysPoll()
	{
		const FSTree* view = GetFile()->ParentRef().get();
		
		ButtonMap::const_iterator it = gButtonMap.find( view );
		
		const bool readable =    it == gButtonMap.end()
		                      || !it->second.installed
		                      || it->second.seed != itsSeed;
		
		return readable * kPollRead | kPollWrite;
	}
	
	ssize_t Button_socket_Handle::SysRead( char* buffer, std::size_t byteCount )
	{
		const FSTree* view = GetFile()->ParentRef().get();
		
	retry:
		
		ButtonMap::const_iterator it = gButtonMap.find( view );
		
		if ( it == gButtonMap.end()  ||  !it->second.installed )
		{
			p7::throw_errno( ECONNRESET );
		}
		
		const Button_Parameters& params = it->second;
		
		if ( params.seed == itsSeed )
		{
			TryAgainLater();
			
			goto retry;
		}
		
		if ( byteCount == 0 )
		{
			return 0;
		}
		
		itsSeed = params.seed;
		
		const char c = '\n';
		
		buffer[ 0 ] = c;
		
		return sizeof c;
	}
	
	
	class FSTree_Button_socket : public FSTree
	{
		public:
			FSTree_Button_socket( const FSTreePtr&    parent,
	                              const std::string&  name ) : FSTree( parent, name )
			{
			}
			
			mode_t FilePermMode() const  { return S_IRUSR; }
			
			boost::shared_ptr< IOHandle > Open( OpenFlags flags ) const;
	};
	
	boost::shared_ptr< IOHandle > FSTree_Button_socket::Open( OpenFlags flags ) const
	{
		const FSTree* view = ParentRef().get();
		
		ButtonMap::const_iterator it = gButtonMap.find( view );
		
		if ( it == gButtonMap.end()  ||  !it->second.installed )
		{
			p7::throw_errno( ECONNREFUSED );
		}
		
		IOHandle* result = new Button_socket_Handle( Self(), flags );
		
		return boost::shared_ptr< IOHandle >( result );
	}
	
	
	template < class Property >
	static FSTreePtr Property_Factory( const FSTreePtr&    parent,
	                                   const std::string&  name )
	{
		return New_FSTree_Property( parent,
		                            name,
		                            &Property::Get,
		                            &Property::Set );
	}
	
	static const FSTree_Premapped::Mapping local_mappings[] =
	{
		{ "title", &Property_Factory< Button_Title > },
		
		{ "socket", &Basic_Factory< FSTree_Button_socket > },
		
		{ NULL, NULL }
	};
	
	FSTreePtr New_FSTree_new_button( const FSTreePtr& parent, const std::string& name )
	{
		return FSTreePtr( new FSTree_new_View( parent,
		                                       name,
		                                       &CreateView,
		                                       local_mappings,
		                                       &DestroyDelegate ) );
	}
	
}

