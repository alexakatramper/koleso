//
//  main.cpp
//  txt2html
//
//  Created by Alexey Schutsky on 10/9/14.
//  Copyright (c) 2014 Alexey Schutsky. All rights reserved.
//

#include <stdio.h>
#include <string>
#include <stdexcept>

using std::string;
using std::runtime_error;

#define MAX_LEN	1024*16

#define __PLACEHOLDER_AUTHOR__	"__PLACEHOLDER_AUTHOR__"
#define __PLACEHOLDER_TITLE__	"__PLACEHOLDER_TITLE__"
#define __PLACEHOLDER_CONTENT__	"__PLACEHOLDER_CONTENT__"

//------------------------------------------------------------------------------
void printHelp()
{
	printf( "txt2html v.0.1\n" );
	printf( "usage: txt2html <source_file>\n" );
	printf( "usage: txt2html -l <list_file>\n\n" );
}


//------------------------------------------------------------------------------
char* trimSpaces( char* str )
{
	char* res = str;
	
	while( isspace( * res ) )
	{
		++res;
	}
	
	size_t last = strlen( res ) - 1;
	while( isspace( res[last] ) )
	{
		res[last] = 0;
		--last;
	}
	
	return res;
}


//------------------------------------------------------------------------------
void replaceSymbols( string& str )
{
	// '&' -> &amp; - should be the first as '& is a 'magic' symbol
	size_t pos = str.find( "&", 0 );
	
	while( pos != string::npos )
	{
		str.replace( pos, 1, "&amp;" );
		pos += 4;
		pos = str.find( "&", pos );
	}

	// '<' -> &lt;
	pos = str.find( "<", 0 );
	
	while( pos != string::npos )
	{
		str.replace( pos, 1, "&lt;" );
		pos += 4;
		pos = str.find( "<", pos );
	}
	
	// '>' -> &gt;
	pos = str.find( ">", 0 );
	
	while( pos != string::npos )
	{
		str.replace( pos, 1, "&gt;" );
		pos += 4;
		pos = str.find( ">", pos );
	}
	

	// '"' -> &quot;
	pos = str.find( "\"", 0 );
	
	while( pos != string::npos )
	{
		str.replace( pos, 1, "&quot;" );
		pos = str.find( "\"", pos );
	}


}


//------------------------------------------------------------------------------
int processFile( string srcPath )
{
	int res = 0;
	FILE*	src = 0;
	FILE*	dst = 0;

	char* str = new char[MAX_LEN];

	
	try
	{
		string dstPath = srcPath;
		size_t pos = dstPath.rfind( ".txt" );
		if( pos == string::npos )
		{
			throw std::runtime_error( "invalid source file name (no '.txt' suffix)" );
		}
		
		dstPath.replace( pos, 4, ".html" );
		
		src = fopen( srcPath.c_str(), "r" );
		if( src == 0 )
		{
			throw std::runtime_error( "can't open resource file" );
		}
		
		dst = fopen( dstPath.c_str(), "w" );
		if( dst == 0 )
		{
			throw std::runtime_error( "can't open destination file" );
		}
		
		while( !feof( src ) )
		{
			memset( str, 0, MAX_LEN );
			fgets( str, MAX_LEN, src );
			
			string dstStr = trimSpaces( str );
			replaceSymbols( dstStr );
			
			if( dstStr.length() == 0 )
			{
				fprintf( dst, "<br />\n" );
			}
			else
			{
				fprintf( dst, "<p>%s</p>\n", dstStr.c_str() );
			}
		}
	}
	catch( std::runtime_error e )
	{
		res = -1;
		printf( "ERROR: %s\n", e.what() );
	}
	
	if( str )
		delete[] str;
	
	if( src )
		fclose( src );
	
	if( dst )
		fclose( dst );
	
	return res;
}



//------------------------------------------------------------------------------
int processList( string listPath )
{
	int res = 0;

	FILE* f = 0;
	
	char path[FILENAME_MAX];
	
	try
	{
		f = fopen( listPath.c_str(), "r" );
		if( f == 0 )
		{
			throw std::runtime_error( "can't open list file" );
		}
		
		while( !feof( f ) )
		{
			memset( path, 0, FILENAME_MAX );
			fgets( path, FILENAME_MAX, f );
			
			trimSpaces( path );
			
			if( path[0] == '#' || path[0] == ';' )
				continue;
			
			if( strlen( path ) == 0 )
				continue;
			
			printf( "processing: %s\n", path );
			
			processFile( path );
		}
	}
	catch( std::runtime_error e )
	{
		res = -1;
		printf( "ERROR: %s\n", e.what() );
	}

	printf( "done\n" );
	
	return res;
}


//------------------------------------------------------------------------------
int main( int argc, const char * argv[] )
{

	int res = 0;
	
	if( argc == 2 )
	{
		res = processFile( argv[1] );
	}
	else if( argc == 3 )
	{
		if( strcmp( argv[1], "-l" ) == 0 )
		{
			res = processList( argv[2] );
		}
		else
		{
			printHelp();
			return -1;
		}
	}
	else
	{
		printHelp();
		return -1;
	}

	return res;
}
