/*
 WString.cpp - String library for Wiring & Arduino
 ...mostly rewritten by Paul Stoffregen...
 Copyright (c) 2009-10 Hernando Barragan.  All rights reserved.
 Copyright 2011, Paul Stoffregen, paul@pjrc.com
 Modified by Ivan Grokhotkov, 2014 - ESP31B support
 Modified by Michael C. Miller, 2015 - ESP31B progmem support

 This library is free software; you can redistribute it and/or
 modify it under the terms of the GNU Lesser General Public
 License as published by the Free Software Foundation; either
 version 2.1 of the License, or (at your option) any later version.

 This library is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public
 License along with this library; if not, write to the Free Software
 Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 */

//#include "esp_log.h"
#include "String.h"

//static const char LOGTAG[] = "String";

/*
// debugging method
void String::dump() {
    ESP_LOGI(LOGTAG, "dump: c=%d, l=%d, ptr=%p", capacity, len, buffer);
}*/

String::String(long value, unsigned char base)
{
    init();
    char buf[2 + 8 * sizeof(long)];
    ltoa(value, buf, base);
    *this = buf;
}

String::String(unsigned long value, unsigned char base)
{
    init();
    char buf[1 + 8 * sizeof(unsigned long)];
    ultoa(value, buf, base);
    *this = buf;
}

String::String(float value, unsigned char decimalPlaces)
{
    init();
    char buf[33];
    *this = dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf);
}

String::String(double value, unsigned char decimalPlaces)
{
    init();
    char buf[33];
    *this = dtostrf(value, (decimalPlaces + 2), decimalPlaces, buf);
}

/*********************************************/
/*  Concatenate                              */
/*********************************************/

StringSumHelper & operator +(const StringSumHelper &lhs, const String &rhs)
{
    StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
    if(!a.concat(rhs.buffer, rhs.len)) {
        a.invalidate();
    }
    return a;
}

StringSumHelper & operator +(const StringSumHelper &lhs, const char *cstr)
{
    StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
    if(!cstr || !a.concat(cstr, strlen(cstr))) {
        a.invalidate();
    }
    return a;
}

StringSumHelper & operator +(const StringSumHelper &lhs, char c)
{
    StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
    if(!a.concat(c)) {
        a.invalidate();
    }
    return a;
}

StringSumHelper & operator +(const StringSumHelper &lhs, unsigned char num)
{
    StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
    if(!a.concat(num)) {
        a.invalidate();
    }
    return a;
}

StringSumHelper & operator +(const StringSumHelper &lhs, int num)
{
    StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
    if(!a.concat(num)) {
        a.invalidate();
    }
    return a;
}

StringSumHelper & operator +(const StringSumHelper &lhs, unsigned int num)
{
    StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
    if(!a.concat(num)) {
        a.invalidate();
    }
    return a;
}

StringSumHelper & operator +(const StringSumHelper &lhs, long num)
{
    StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
    if(!a.concat(num)) {
        a.invalidate();
    }
    return a;
}

StringSumHelper & operator +(const StringSumHelper &lhs, unsigned long num)
{
    StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
    if(!a.concat(num)) {
        a.invalidate();
    }
    return a;
}

StringSumHelper & operator +(const StringSumHelper &lhs, float num)
{
    StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
    if(!a.concat(num)) {
        a.invalidate();
    }
    return a;
}

StringSumHelper & operator +(const StringSumHelper &lhs, double num)
{
    StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
    if(!a.concat(num)) {
        a.invalidate();
    }
    return a;
}

StringSumHelper & operator + (const StringSumHelper &lhs, const __FlashStringHelper *rhs)
{
    StringSumHelper &a = const_cast<StringSumHelper&>(lhs);
    if (!a.concat(rhs))	a.invalidate();
    return a;
}

// /*********************************************/
// /*  Comparison                               */
// /*********************************************/

void String::getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index) const
{
    if(!bufsize || !buf) {
        return;
    }
    if(index >= len) {
        buf[0] = 0;
        return;
    }
    unsigned int n = bufsize - 1;
    if(n > len - index) {
        n = len - index;
    }
    strncpy((char *) buf, buffer + index, n);
    buf[n] = 0;
}

// /*********************************************/
// /*  Search                                   */
// /*********************************************/

int String::indexOf(char c) const
{
    return indexOf(c, 0);
}

int String::indexOf(char ch, unsigned int fromIndex) const
{
    if(fromIndex >= len) {
        return -1;
    }
    const char* temp = strchr(buffer + fromIndex, ch);
    if(temp == NULL) {
        return -1;
    }
    return temp - buffer;
}

int String::indexOf(const String &s2) const
{
    return indexOf(s2, 0);
}

int String::indexOf(const String &s2, unsigned int fromIndex) const
{
    if(fromIndex >= len) {
        return -1;
    }
    const char *found = strstr(buffer + fromIndex, s2.buffer);
    if(found == NULL) {
        return -1;
    }
    return found - buffer;
}

int String::lastIndexOf(char theChar) const
{
    return lastIndexOf(theChar, len - 1);
}

int String::lastIndexOf(char ch, unsigned int fromIndex) const
{
    if(fromIndex >= len) {
        return -1;
    }
    char tempchar = buffer[fromIndex + 1];
    buffer[fromIndex + 1] = '\0';
    char* temp = strrchr(buffer, ch);
    buffer[fromIndex + 1] = tempchar;
    if(temp == NULL) {
        return -1;
    }
    return temp - buffer;
}

int String::lastIndexOf(const String &s2) const
{
    return lastIndexOf(s2, len - s2.len);
}

int String::lastIndexOf(const String &s2, unsigned int fromIndex) const
{
    if(s2.len == 0 || len == 0 || s2.len > len) {
        return -1;
    }
    if(fromIndex >= len) {
        fromIndex = len - 1;
    }
    int found = -1;
    for(char *p = buffer; p <= buffer + fromIndex; p++) {
        p = strstr(p, s2.buffer);
        if(!p) {
            break;
        }
        if((unsigned int) (p - buffer) <= fromIndex) {
            found = p - buffer;
        }
    }
    return found;
}

String String::substring(unsigned int left, unsigned int right) const
{
    if(left > right) {
        unsigned int temp = right;
        right = left;
        left = temp;
    }
    String out;
    if(left >= len) {
        return out;
    }
    if(right > len) {
        right = len;
    }
    char temp = buffer[right];  // save the replaced character
    buffer[right] = '\0';
    out = buffer + left;  // pointer arithmetic
    buffer[right] = temp;  //restore character
    return out;
}

// /*********************************************/
// /*  Modification                             */
// /*********************************************/

void String::replace(char find, char replace)
{
    if(!buffer) {
        return;
    }
    for(char *p = buffer; *p; p++) {
        if(*p == find) {
            *p = replace;
        }
    }
}

void String::replace(const String& find, const String& replace)
{
    if(len == 0 || find.len == 0) {
        return;
    }
    int diff = replace.len - find.len;
    char *readFrom = buffer;
    char *foundAt;
    if(diff == 0) {
        while((foundAt = strstr(readFrom, find.buffer)) != NULL) {
            memcpy(foundAt, replace.buffer, replace.len);
            readFrom = foundAt + replace.len;
        }
    } else if(diff < 0) {
        char *writeTo = buffer;
        while((foundAt = strstr(readFrom, find.buffer)) != NULL) {
            unsigned int n = foundAt - readFrom;
            memcpy(writeTo, readFrom, n);
            writeTo += n;
            memcpy(writeTo, replace.buffer, replace.len);
            writeTo += replace.len;
            readFrom = foundAt + find.len;
            len += diff;
        }
        strcpy(writeTo, readFrom);
    } else {
        unsigned int size = len; // compute size needed for result
        while((foundAt = strstr(readFrom, find.buffer)) != NULL) {
            readFrom = foundAt + find.len;
            size += diff;
        }
        if(size == len) {
            return;
        }
        if(size > capacity && !resize(size)) {
            return;    // XXX: tell user!
        }
        int index = len - 1;
        while(index >= 0 && (index = lastIndexOf(find, index)) >= 0) {
            readFrom = buffer + index + find.len;
            memmove(readFrom + diff, readFrom, len - (readFrom - buffer));
            len += diff;
            buffer[len] = 0;
            memcpy(buffer + index, replace.buffer, replace.len);
            index--;
        }
    }
}

void String::remove(unsigned int index)
{
    // Pass the biggest integer as the count. The remove method
    // below will take care of truncating it at the end of the
    // string.
    remove(index, (unsigned int) -1);
}

void String::remove(unsigned int index, unsigned int count)
{
    if(index >= len) {
        return;
    }
    if(count <= 0) {
        return;
    }
    if(count > len - index) {
        count = len - index;
    }
    char *writeTo = buffer + index;
    len = len - count;
    strncpy(writeTo, buffer + index + count, len - index);
    buffer[len] = 0;
}

void String::toLowerCase(void)
{
    if(!buffer) {
        return;
    }
    for(char *p = buffer; *p; p++) {
        *p = tolower(*p);
    }
}

void String::toUpperCase(void)
{
    if(!buffer) {
        return;
    }
    for(char *p = buffer; *p; p++) {
        *p = toupper(*p);
    }
}

void String::trim(void)
{
    if(!buffer || len == 0) {
        return;
    }
    char *begin = buffer;
    while(isspace(*begin)) {
        begin++;
    }
    char *end = buffer + len - 1;
    while(isspace(*end) && end >= begin) {
        end--;
    }
    len = end + 1 - begin;
    if(begin > buffer) {
        memcpy(buffer, begin, len);
    }
    buffer[len] = 0;
}
