/*
	menus.hh
	--------
*/

#include "mac_ui/menus.hh"


namespace mac {
namespace ui  {

void delete_all_menu_items( MenuRef menu )
{
#if ! OPAQUE_TOOLBOX_STRUCTS
	
	menu[0]->menuWidth  = 0;
	menu[0]->menuHeight = 0;
	
	unsigned char* p = menu[0]->menuData;
	
	p += 1 + p[ 0 ];
	
	*p++ = '\0';
	
	SetHandleSize( (Handle) menu, p - (unsigned char*) *menu );
	
	return;
	
#endif
	
	uint16_t n = CountMenuItems( menu );
	
	while ( n > 0 )
	{
		DeleteMenuItem( menu, n );
		
		--n;
	}
}

}
}
