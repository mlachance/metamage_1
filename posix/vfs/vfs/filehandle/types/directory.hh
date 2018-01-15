/*
	directory.hh
	------------
*/

#ifndef VFS_FILEHANDLE_TYPES_DIRECTORY_HH
#define VFS_FILEHANDLE_TYPES_DIRECTORY_HH

// vfs
#include "vfs/dir_contents_box.hh"
#include "vfs/filehandle.hh"
#include "vfs/node_fwd.hh"
#include "vfs/filehandle/methods/stream_method_set.hh"

// POSIX
//#include <dirent.h>
struct dirent;


namespace vfs
{
	
	class dir_contents;
	
	struct filehandle_method_set;
	
	extern const stream_method_set dir_stream_methods;
	
	
	struct dir_handle_extra
	{
		filehandle_destructor  chained_destructor;
		dir_contents*          contents;
	};
	
	void destroy_dir_handle( filehandle* that );
	
	
	class dir_handle : public filehandle
	{
		private:
			dir_contents_box  its_contents;
		
		public:
			dir_handle( const node* dir, filehandle_destructor dtor = 0 );  // NULL
			
			dir_handle( const filehandle_method_set& methods );
			
			~dir_handle();
			
			int readdir( dirent& entry );
	};
	
}

#endif
