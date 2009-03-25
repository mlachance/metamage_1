/*	==========
 *	MacFile.cc
 *	==========
 */

#include "Genie/IO/MacFile.hh"

// Genie
#include "Genie/FileSystem/FSSpec.hh"
#include "Genie/FileSystem/FSTree_RsrcFile.hh"
#include "Genie/IO/RegularFile.hh"
#include "Genie/Utilities/AsyncIO.hh"


namespace Genie
{
	
	namespace N = Nitrogen;
	namespace NN = Nucleus;
	
	
	class MacFileHandle : public RegularFileHandle
	{
		private:
			typedef FSTreePtr (*FileGetter)( const FSSpec& );
			
			Nucleus::Shared< Nitrogen::FSFileRefNum >  itsRefNum;
			FileGetter                                 itsFileGetter;
		
		public:
			MacFileHandle( const Nucleus::Shared< Nitrogen::FSFileRefNum >&  refNum,
			               OpenFlags                                         flags,
			               FileGetter                                        getFile );
			
			~MacFileHandle();
			
			FSTreePtr GetFile() const;
			
			boost::shared_ptr< IOHandle > Clone();
			
			ssize_t SysRead( char* data, std::size_t byteCount );
			
			ssize_t SysWrite( const char* data, std::size_t byteCount );
			
			//void IOCtl( unsigned long request, int* argp );
			
			off_t GetEOF() const  { return Nitrogen::GetEOF( itsRefNum ); }
			
			void SetEOF( off_t length )  { Nitrogen::SetEOF( itsRefNum, length ); }
	};
	
	
	static FSSpec FSSpecFromFRefNum( N::FSFileRefNum refNum )
	{
		FSSpec result;
		
		FCBPBRec pb;
		
		pb.ioVRefNum = 0;
		pb.ioFCBIndx = 0;
		pb.ioRefNum  = refNum;
		pb.ioNamePtr = result.name;
		
		N::ThrowOSStatus( ::PBGetFCBInfoSync( &pb ) );
		
		result.vRefNum = pb.ioFCBVRefNum;
		result.parID   = pb.ioFCBParID;
		
		return result;
	}
	
	MacFileHandle::MacFileHandle( const NN::Shared< N::FSFileRefNum >&  refNum,
	                              OpenFlags                             flags,
	                              FileGetter                            getFile )
	: RegularFileHandle( flags  ),
	  itsRefNum        ( refNum ),
	  itsFileGetter    ( getFile )
	{
	}
	
	MacFileHandle::~MacFileHandle()
	{
	}
	
	FSTreePtr MacFileHandle::GetFile() const
	{
		return itsFileGetter( FSSpecFromFRefNum( itsRefNum ) );
	}
	
	boost::shared_ptr< IOHandle > MacFileHandle::Clone()
	{
		return boost::shared_ptr< IOHandle >( new MacFileHandle( itsRefNum,
		                                                         GetFlags(),
		                                                         itsFileGetter ) );
	}
	
	ssize_t MacFileHandle::SysRead( char* data, std::size_t byteCount )
	{
		ssize_t read = FSRead( itsRefNum,
		                       N::fsFromStart,
		                       GetFileMark(),
		                       byteCount,
		                       data,
		                       ThrowEOF_Never() );
		
		return Advance( read );
	}
	
	ssize_t MacFileHandle::SysWrite( const char* data, std::size_t byteCount )
	{
		const bool appending = GetFlags() & O_APPEND;
		
		const N::FSIOPosMode  mode   = appending ? N::fsFromLEOF : N::fsFromStart;
		const SInt32          offset = appending ? 0             : GetFileMark();
		
		ssize_t written = FSWrite( itsRefNum,
		                           mode,
		                           offset,
		                           byteCount,
		                           data );
		
		return Advance( written );
	}
	
	boost::shared_ptr< IOHandle >
	//
	New_DataForkHandle( const NN::Shared< N::FSFileRefNum >&  refNum,
	                    OpenFlags                             flags )
	{
		return boost::shared_ptr< IOHandle >( new MacFileHandle( refNum,
		                                                         flags,
		                                                         &FSTreeFromFSSpec ) );
	}
	
	boost::shared_ptr< IOHandle >
	//
	New_RsrcForkHandle( const NN::Shared< N::FSFileRefNum >&  refNum,
	                    OpenFlags                             flags )
	{
		return boost::shared_ptr< IOHandle >( new MacFileHandle( refNum,
		                                                         flags,
		                                                         &GetRsrcForkFSTree ) );
	}
	
}

