//
//  BaseAlgorithm.h
//  ApparkTest
//
//  Created by XiaoFeng on 12-2-1.
//  Copyright (c) 2012年 PunchBox. All rights reserved.
//

#ifndef VitApparkTest_Base64_h
#define VitApparkTest_Base64_h

const char  Vittable[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

unsigned int VitEnBase64(unsigned char * dest, const unsigned char * src, unsigned int src_size);
unsigned int VitUnBase64(unsigned char * dest, const unsigned char * src, unsigned int src_size);

void Vitbtea(unsigned int *v, int n, unsigned int const key[4]);

char *Vittcurlencode(const char *ptr, int size);
char *Vittcurldecode(const char *str, int *sp);

#endif
