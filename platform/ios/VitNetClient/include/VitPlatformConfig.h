/*
 * Copyright (c) 2012 Chukong Technologies, Inc.
 *
 * http://www.sweetpome.com
 * http://tools.cocoachina.com
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to permit
 * persons to whom the Software is furnished to do so, subject to the
 * following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
 * NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
 * DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
 * OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
 * USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef _VIT_PLATFORMCONFIG_H_LEE_
#define _VIT_PLATFORMCONFIG_H_LEE_

#define VIT_PLATFORM_UNKNOWN            0
#define VIT_PLATFORM_IOS                1
#define VIT_PLATFORM_ANDROID            2
#define VIT_PLATFORM_WIN32              3
#define VIT_PLATFORM_LINUX              4

#define VIT_TARGET_PLATFORM   VIT_PLATFORM_IOS

// ios
#if ! VIT_TARGET_PLATFORM && (defined(TARGET_OS_IPHONE) || defined(TARGET_OS_SIMULATOR))
	#undef  VIT_TARGET_PLATFORM
	#define VIT_TARGET_PLATFORM         VIT_PLATFORM_IOS
	#define VIT_SUPPORT_PVRTC
#endif

// android
#if ! VIT_TARGET_PLATFORM && defined(ANDROID)
	#undef  VIT_TARGET_PLATFORM
	#define VIT_TARGET_PLATFORM         VIT_PLATFORM_ANDROID
#endif

// win32
#if ! VIT_TARGET_PLATFORM && (defined(WIN32) && defined(_WINDOWS))
	#undef  VIT_TARGET_PLATFORM
	#define VIT_TARGET_PLATFORM         VIT_PLATFORM_WIN32
#endif

// linux
#if ! VIT_TARGET_PLATFORM && defined(LINUX)
	#undef  VIT_TARGET_PLATFORM
	#define VIT_TARGET_PLATFORM         VIT_PLATFORM_LINUX
#endif

// check user set platform
#if ! VIT_TARGET_PLATFORM
	#error  "Can not recognize the target platform, compling under a unsupported platform?"
#endif 

#if (VIT_TARGET_PLATFORM == VIT_PLATFORM_WIN32)
#pragma warning (disable:4127) 
#pragma warning (disable:4819) 
#endif  

#endif	// _VIT_PLATFORMCONFIG_H_LEE_

