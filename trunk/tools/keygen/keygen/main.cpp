//
//  main.cpp
//  keygen
//
//  Created by Alexey Schutsky on 9/28/14.
//  Copyright (c) 2014 Alexey Schutsky. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include <vector>

using std::vector;

void extractPass( char* key, char* pass )
{
	memset( pass, 0, 17 );
	for( int i = 0; i < 16; ++i )
	{
		pass[key[i*2+1]] = key[i*2];
		if( pass[key[i*2+1]] < 0x0A )
			pass[key[i*2+1]] += '0';
		else
			pass[key[i*2+1]] += 'a' - 0x0A;
	}
}


int main(int argc, const char * argv[])
{
	char pass[17] = { 0 };
	for( int n = 0; n < 10; ++ n )
	{
		char key[33] = { 0 };
		
		time_t now;
		time( &now );
		
		srand( (unsigned int)now );
		
		vector<int> nums;
		
		nums.resize( 16 );
		
		int i = 0;
		for( i = 0; i < 16; ++i )
		{
			nums[i] = i;
		}

		random_shuffle( nums.begin(), nums.end() );

		for( i = 0; i < 16; ++i )
		{
			key[i*2] = rand() % 15;
			key[i*2 + 1] = nums[i];
		}

		key[32] = 0;
		
		for( i = 0; i < 32; ++i )
		{
//			if( (i > 0 ) && ( i % 8 == 0 ) )
//				printf( "-" );
			printf( "%X", key[i] );
		}
		
		extractPass( key, pass );
		printf( " : %s", pass );
		printf( "\n" );
	}
	return 0;
}
