/*	==================
 *	ExtractIncludes.cc
 *	==================
 */

#include "A-line/ExtractIncludes.hh"

// Iota
#include "iota/strings.hh"

// text-input
#include "text_input/feed.hh"
#include "text_input/get_line_from_feed.hh"

// poseven
#include "poseven/extras/fd_reader.hh"
#include "poseven/functions/open.hh"
#include "poseven/functions/write.hh"

// BitsAndBytes
#include "StringPredicates.hh"


namespace tool
{
	
	namespace NN = Nucleus;
	namespace p7 = poseven;
	
	using BitsAndBytes::eos;
	
	
	static void ExtractInclude( const std::string& line, IncludesCache& includes )
	{
		struct BadIncludeDirective {};
		
		std::size_t pos = line.find_first_not_of( " \t" );
		
		if ( !eos( pos )  &&  line[ pos ] == '#' )
		{
			std::string include = "include";
			
			if ( line.substr( pos + 1, include.size() ) == include )
			{
				try
				{
					pos = line.find_first_not_of( " \t", pos + 1 + include.size() );
					
					if ( eos( pos ) )  throw BadIncludeDirective();
					
					char c;
					
					if ( line[ pos ] == '"' )
					{
						c = '"';
					}
					else if ( line[ pos ] == '<' )
					{
						c = '>';
					}
					else
					{
						// Not "" or <>, maybe a macro
						//throw BadIncludeDirective();
						return;
					}
					
					++pos;
					std::size_t end = line.find( c, pos );
					
					if ( eos( end ) )  throw BadIncludeDirective();
					
					std::vector< std::string >& v( c == '"' ? includes.user : includes.system );
					
					v.push_back( line.substr( pos, end - pos ) );
				}
				catch ( const BadIncludeDirective& )
				{
					p7::write( p7::stderr_fileno, STR_LEN( "Bad include!\n" ) );
				}
				catch ( ... )
				{
					p7::write( p7::stderr_fileno, STR_LEN( "Bad compiler!\n" ) );
				}
			}
		}
	}
	
	IncludesCache ExtractIncludes( const std::string& pathname )
	{
		text_input::feed feed;
		
		NN::Owned< p7::fd_t > fd = p7::open( pathname, p7::o_rdonly );
		
		p7::fd_reader reader( fd );
		
		IncludesCache includes;
		
		while ( const std::string* s = get_line_from_feed( feed, reader ) )
		{
			std::string line( s->begin(), s->end() - 1 );
			
			ExtractInclude( line, includes );
		}
		
		return includes;
	}
	
}

