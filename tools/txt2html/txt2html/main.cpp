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

#define MAX_LEN	1024

#define __PLACEHOLDER_AUTHOR__	"__PLACEHOLDER_AUTHOR__"
#define __PLACEHOLDER_TITLE__	"__PLACEHOLDER_TITLE__"
#define __PLACEHOLDER_CONTENT__	"__PLACEHOLDER_CONTENT__"

//------------------------------------------------------------------------------
void printHelp()
{
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
	size_t pos = str.find( "\"", 0 );
	
	while( pos != string::npos )
	{
		str.replace( pos, 1, "&quot;" );
		pos = str.find( "\"", pos );
	}
	
}


//------------------------------------------------------------------------------
int main( int argc, const char * argv[] )
{
	
	FILE*	src = 0;
	FILE*	dst = 0;
	
	try
	{
		char str[MAX_LEN];

//		const char*	tplPath = "/Users/schutsky/Documents/Projects/ArtisticUkraine/Koleso/Koleso/books/koleso-001/article-template-middle.html";
		string srcPath = "/Users/schutsky/Documents/Projects/ArtisticUkraine/classic/Васильченко Степан Васильович/Степан Васильченко - На світанні.txt";
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
		printf( "ERROR: %s\n", e.what() );
	}
	
	
	if( src )
		fclose( src );
	
	if( dst )
		fclose( dst );
	
    return 0;
}
