
//
//  BaseAlgorithm.cpp
//  ApparkTest
//
//  Created by XiaoFeng on 12-2-1.
//  Copyright (c) 2012年 PunchBox. All rights reserved.
//

#include "VitPcheader.h"
#include "VitBaseAlgorithm.h"
#include "VitConnectDefine.h"



bool_t Vitisbase64(char c);
char *Vittcstrdup(const void *str);


unsigned int VitEnBase64(unsigned char * dest, const unsigned char * src, unsigned int src_size)
{
    unsigned char * destOrg = dest;
    while(src_size)
    {
        *dest++ = Vittable[(src[0] >> 2) & 0x3f];
        *dest++ = Vittable[((src[0] << 4) + (--src_size ? src[1] >> 4 : 0)) & 0x3f];
        *dest++ = (src_size ? Vittable[((src[1] << 2) + (--src_size ? src[2] >> 6 : 0)) & 0x3f] : '=');
        *dest++ = src_size ? Vittable[src[2] & 0x3f] : '=';
        if(src_size)
            src_size--;
        if(src_size)
            src += 3;
    }
    return dest - destOrg;
}


bool_t Vitisbase64(char c)
{
    return c && strchr(Vittable, c) !=  0;
}


inline unsigned char Vitvalue(char c)
{
    const char *p = strchr(Vittable, c);

    if(p)
        return p-Vittable;
    else
        return 0;
}


unsigned int VitUnBase64(unsigned char *dest, const unsigned char * src, unsigned int src_size)
{
    *dest = 0;

    if(*src == 0) 
    {
        return 0;
    }

    unsigned char *p = dest;

    do
    {
        unsigned char a = Vitvalue(src[0]);
        unsigned char b = Vitvalue(src[1]);
        unsigned char c = Vitvalue(src[2]);
        unsigned char d = Vitvalue(src[3]);
        *p++ = (a << 2) | (b >> 4);
        *p++ = (b << 4) | (c >> 2);
        *p++ = (c << 6) | d;
        if(!Vitisbase64(src[1])) 
        {
            p -= 2;
            break;
        } 
        else if(!Vitisbase64(src[2])) 
        {
            p -= 2;
            break;
        } 
        else if(!Vitisbase64(src[3])) 
        {
            p--;
            break;
        }
        src += 4;
        while(*src && (*src == 13 || *src == 10))
            src++;
    } while(src_size-= 4);

    *p = 0;

    return p-dest;
}


#define DELTA 0x9e3779b9
#define MX (((z>>5^y<<2) + (y>>3^z<<4)) ^ ((sum^y) + (key[(p&3)^e] ^ z)))

void Vitbtea(unsigned int *v, int n, unsigned int const key[4])
{
    unsigned int y, z, sum;
    unsigned p, rounds, e;
    if (n > 1)                      // Coding Part
    {
        rounds = 6 + 52/n;
        sum = 0;
        z = v[n-1];
        do
        {
            sum += DELTA;
            e = (sum >> 2) & 3;
            for (p=0; p<n-1; p++)
            {
                y = v[p+1]; 
                z = v[p] += MX;
            }
            y = v[0];
            z = v[n-1] += MX;
        } while (--rounds);
    }
    else if (n < -1)                // Decoding Part
    {
        n = -n;
        rounds = 6 + 52/n;
        sum = rounds*DELTA;
        y = v[0];
        do
        {
            e = (sum >> 2) & 3;
            for (p=n-1; p>0; p--)
            {
                z = v[p-1];
                y = v[p] -= MX;
            }
            z = v[n-1];
            y = v[0] -= MX;
        } while ((sum -= DELTA) != 0);
    }
}


/* Duplicate a string on memory. */
char *Vittcstrdup(const void *str)
{
    int size = strlen((char *)str);
    char *p;
    p = (char *)malloc(size + 1);
    memset(p, 0, size + 1);
    memcpy(p, str, size);
    p[size] = '\0';
    return p;
}

/* Encode a serial object with URL encoding. */
char *Vittcurlencode(const char *ptr, int size)
{
    char *buf;
    buf = (char *)malloc(size * 3 + 1);
    memset(buf, 0, size * 3 + 1);
    char *wp = buf;
    for(int i = 0; i < size; i++){
        int c = ((unsigned char *)ptr)[i];
        if((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z') ||
           (c >= '0' && c <= '9') || (c != '\0' && strchr("_-.!~*'()", c))){
            *(wp++) = c;
        } else {
            wp += sprintf(wp, "%%%02X", c);
        }
    }
    *wp = '\0';
    return buf;
}


/* Decode a string encoded with URL encoding. */
char *Vittcurldecode(const char *str, int *sp)
{
    char *buf = Vittcstrdup(str);
    char *wp = buf;
    while(*str != '\0'){
        if(*str == '%'){
            str++;
            if(((str[0] >= '0' && str[0] <= '9') || (str[0] >= 'A' && str[0] <= 'F') ||
                (str[0] >= 'a' && str[0] <= 'f')) &&
               ((str[1] >= '0' && str[1] <= '9') || (str[1] >= 'A' && str[1] <= 'F') ||
                (str[1] >= 'a' && str[1] <= 'f'))){
                   unsigned char c = *str;
                   if(c >= 'A' && c <= 'Z') c += 'a' - 'A';
                   if(c >= 'a' && c <= 'z'){
                       *wp = c - 'a' + 10;
                   } else {
                       *wp = c - '0';
                   }
                   *wp *= 0x10;
                   str++;
                   c = *str;
                   if(c >= 'A' && c <= 'Z') c += 'a' - 'A';
                   if(c >= 'a' && c <= 'z'){
                       *wp += c - 'a' + 10;
                   } else {
                       *wp += c - '0';
                   }
                   str++;
                   wp++;
               } else {
                   break;
               }
        } else if(*str == '+'){
            *wp = ' ';
            str++;
            wp++;
        } else {
            *wp = *str;
            str++;
            wp++;
        }
    }
    *wp = '\0';
    *sp = wp - buf;
    return buf;
}

