//
//  NSString+UUID.m
//  Baker
//  See: http://oleb.net/blog/2011/09/how-to-replace-the-udid/
//
//  ==========================================================================================
//
//  Copyright (c) 2010-2013, Davide Casali, Marco Colombo, Alessandro Morandi
//  Copyright (c) 2014, Andrew Krowczyk, Cédric Mériau
//  All rights reserved.
//
//  Redistribution and use in source and binary forms, with or without modification, are
//  permitted provided that the following conditions are met:
//
//  Redistributions of source code must retain the above copyright notice, this list of
//  conditions and the following disclaimer.
//  Redistributions in binary form must reproduce the above copyright notice, this list of
//  conditions and the following disclaimer in the documentation and/or other materials
//  provided with the distribution.
//  Neither the name of the Baker Framework nor the names of its contributors may be used to
//  endorse or promote products derived from this software without specific prior written
//  permission.
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
//  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//

#import "NSString+UUID.h"

#import <SystemConfiguration/CaptiveNetwork.h>
#import <CommonCrypto/CommonDigest.h>
#import <ifaddrs.h>
#import <arpa/inet.h>
#import <sys/socket.h>
#import <sys/types.h>
#import <net/if_dl.h>

@implementation NSString (UUID)

+ (NSString *)uuid
{
    NSString *uuidString = @"";
	// SAB: try to create UUID using MAC adress so it would be the same after app reinstalled
	// -----------
	// Apples recommended technique for getting the MAC address is to use CNCopySupportedInterfaces
	// to get the BSD name of the Wi-Fi interface (typically "en0") and then use
	// getifaddrs and  search for the AF_LINK interface whose ifa_name matches that name.
	
//	NSString* output = @"";
	NSString *address = @"";
	NSString * wifiBSDName = @"en0";
	CFArrayRef interfaceArray = nil;
	
	// For iOS 4.1 and above, grab WIFI BSD name dynamically
	if( [[[UIDevice currentDevice] systemVersion] compare:@"4.1" options:NSNumericSearch] != NSOrderedAscending )
	{
		interfaceArray = CNCopySupportedInterfaces();
		
		if( interfaceArray )
		{
			if( CFArrayGetCount( interfaceArray ) )
			{
				wifiBSDName = (NSString *)CFArrayGetValueAtIndex( interfaceArray, 0 );
			}
		}
	}
	
	struct ifaddrs *interfaces = nil;
	struct ifaddrs *if_addr = nil;
	
	// retrieve the current interfaces - returns 0 on success
	if( getifaddrs( &interfaces ) == 0 )
	{
		// Loop through linked list of interfaces
		if_addr = interfaces;
		while( if_addr != NULL )
		{
			// Check if interface is en0 which is the wifi connection on the iPhone
			if( [[NSString stringWithUTF8String:if_addr->ifa_name] isEqualToString:wifiBSDName] )
			{
				// get MAC address
				const struct sockaddr_dl* sdlAddr = (const struct sockaddr_dl *)if_addr->ifa_addr;
				
				if (sdlAddr)
				{
					const uint8_t * base = (const uint8_t *) &sdlAddr->sdl_data[sdlAddr->sdl_nlen];
					
					address = [address stringByAppendingFormat:@"%02X", base[0]];
					for (int i = 1; i < sdlAddr->sdl_alen; i++)
					{
						address = [address stringByAppendingFormat:@":%02X", base[i]];
					}
					
					// encode MAC with MD5
					unsigned char md5[CC_MD5_DIGEST_LENGTH] = {0};
					
					CC_MD5( [address cStringUsingEncoding:NSASCIIStringEncoding], address.length, md5 );
					
					for( int i = 0; i < CC_MD5_DIGEST_LENGTH; i++ )
					{
						uuidString = [uuidString stringByAppendingFormat:@"%02X", md5[i]];
						if( ( i & 1 ) == 1 && i < CC_MD5_DIGEST_LENGTH - 1 )
							uuidString = [uuidString stringByAppendingString:@"-"];
					}
					
					break;
				}
			}
			
			if_addr = if_addr->ifa_next;
		}
	}
	
	// Free memory
	if( interfaceArray )
	{
		CFRelease( interfaceArray );
	}
	if( interfaces )
	{
		freeifaddrs( interfaces );
	}
	
	// check if UDID created, create random if not
	if( [uuidString length] == 0 )
	{
		CFUUIDRef uuid = CFUUIDCreate(NULL);
		if (uuid) {
			uuidString = (NSString *)CFUUIDCreateString(NULL, uuid);
			CFRelease(uuid);
		}
	}
	
    return [uuidString autorelease];
}

@end
