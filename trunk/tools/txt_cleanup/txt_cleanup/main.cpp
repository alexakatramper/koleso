//
//  main.cpp
//  txt_cleanup
//
//  Created by Alexey Schutsky on 10/13/14.
//  Copyright (c) 2014 Alexey Schutsky. All rights reserved.
//

#include <stdio.h>
#include <string>
#include <stdexcept>

using std::string;
using std::runtime_error;

#define MAX_LEN	16*1024


//------------------------------------------------------------------------------
int main( int argc, const char * argv[] )
{
	
	FILE*	srcFile = 0;
	FILE*	dstFile = 0;
	char*	buf1 = 0;
	char*	buf2 = 0;
	
	try
	{
		buf1 = new char[MAX_LEN];
		buf2 = new char[MAX_LEN];
		
		string srcPath = "/Users/schutsky/Documents/Projects/ArtisticUkraine/classic/Ірчан Мирослав/Ірчан Мирослав - Карпатська ніч.txt";
		string dstPath = srcPath;
		size_t pos = dstPath.rfind( ".txt" );
		if( pos == string::npos )
		{
			throw std::runtime_error( "invalid source file name (no '.txt' suffix)" );
		}
		
		dstPath.insert( pos, ".cleaned" );
		
		srcFile = fopen( srcPath.c_str(), "r" );
		if( srcFile == 0 )
		{
			throw std::runtime_error( "can't open resource file" );
		}
		
		dstFile = fopen( dstPath.c_str(), "w" );
		if( dstFile == 0 )
		{
			throw std::runtime_error( "can't open destination file" );
		}
		
		size_t len1 = 0;
		size_t len2 = 0;
		
		// assume the first line is the author's name
		// and the second one is the title
		// and the third one is empty
		// so keep them as is
		for( int i = 0; i < 3; ++i )
		{
			memset( buf1, 0, MAX_LEN );
			fgets( buf1, MAX_LEN, srcFile );
			len1 = strlen( buf1 );
			fwrite( buf1, len1, 1, dstFile );
		}
		
		// clean-up
		// check two lines:
		// if the second one DOES NOT starts with 'new-line' mark:
		// remove end-line from 1-st, add 'space'

		// if the second one starts with 'new-line' mark (\t here) - leave as is
		
		
		while( !feof( srcFile ) )
		{
			memset( buf2, 0, MAX_LEN );
			len2 = 0;
			
			fgets( buf2, MAX_LEN, srcFile );
			len2 = strlen( buf2 );
			
			//
//			if( ( buf2[0] != '\t' ) || ( len2 <= 1 ) )
			if( buf2[0] != '\t' )
			{
				// remove trailing spaces
				while( isspace( buf1[len1 - 1] ) )
				{
					buf1[--len1] = 0;
				}
				// add one space
				buf1[len1++] = ' ';
			}

			
			fwrite( buf1, len1, 1, dstFile );
			char* tmp = buf1;
			buf1 = buf2;
			buf2 = tmp;
			len1 = len2;
		}

		// save the last line
		fwrite( buf1, len1, 1, dstFile );
	}
	catch( std::runtime_error e )
	{
		printf( "ERROR: %s\n", e.what() );
	}
	
	if( buf1 )
		delete buf1;

	if( buf2 )
		delete buf2;

	if( srcFile )
		fclose( srcFile );
	
	if( dstFile )
		fclose( dstFile );
	
	return 0;
}


