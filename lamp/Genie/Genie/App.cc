/*	============
 *	GenieMain.cc
 *	============
 */

// Nitrogen
#include "Nitrogen/AppleEvents.hh"

// Pedestal
#include "Pedestal/Application.hh"
#include "Pedestal/Commands.hh"

// Nitrogen Extras / AEFramework
#include "AEFramework/AEFramework.h"

// relix-kernel
#include "relix/task/scheduler.hh"

// Genie
#include "Genie/ProcessList.hh"


namespace Genie
{
	
	namespace N = Nitrogen;
	namespace Ped = Pedestal;
	
	
	struct Reply_AppleEvent
	{
		static void Handler( Mac::AppleEvent const&  event,
		                     Mac::AppleEvent&        reply )
		{
			N::ReceiveReply( event );
		}
		
		static void Install_Handler()
		{
			N::AEInstallEventHandler< Handler >( Mac::kCoreEventClass,
			                                     Mac::kAEAnswer ).release();
		}
	};
	
	
	class App : public Ped::Application
	{
		public:
			App();
	};
	
	
	static bool About( Ped::CommandCode )
	{
		spawn_process( "/sbin/about" );
		
		return true;
	}
	
	static bool NewDocument( Ped::CommandCode )
	{
		spawn_process( "/bin/teletype" );
		
		return true;
	}
	
	App::App()
	{
		Ped::gActivelyBusy_Hook = &relix::is_active;
		Ped::gReadyToExit_Hook  = &is_ready_to_exit;
		
		SetCommandHandler( Ped::kCmdAbout, &About       );
		SetCommandHandler( Ped::kCmdNew,   &NewDocument );
		
		Reply_AppleEvent::Install_Handler();
		
		spawn_process( "/etc/startup" );
	}
	
}


//	main
//	----

int main()
{
	Genie::App app;
	
	return app.Run();
}

