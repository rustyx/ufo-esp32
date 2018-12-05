/*
 WString.h - String library for Wiring & Arduino
 ...mostly rewritten by Paul Stoffregen...
 Copyright (c) 2009-10 Hernando Barragan.  All right reserved.
 Copyright 2011, Paul Stoffregen, paul@pjrc.com

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

#ifndef String_class_h
#define String_class_h
#ifdef __cplusplus

#include "stdlib_noniso.h"
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "pgmspace.h"

// An inherited class for holding the result of a concatenation.  These
// result objects are assumed to be writable by subsequent concatenations.
class StringSumHelper;

// an abstract class used as a means to proide a unique pointer type
// but really has no body
class __FlashStringHelper;
#define F(string_literal) (reinterpret_cast<const __FlashStringHelper *>(PSTR(string_literal)))

// The string class
class String
{
    // use a function pointer to allow for "if (s)" without the
    // complications of an operator bool(). for more information, see:
    // http://www.artima.com/cppsource/safebool.html
    typedef void (String::*StringIfHelperType)() const;
    void StringIfHelper() const
    {
    }

public:
    //ADDED!!!
    /* @brief   frees allocated memory and initializes String object.
    */
    void clear() { invalidate(); };

    //ADDED!!!
    /* @brief stream-like printf method that appends the "printf" formatted output to the String data
     * use clear() if you dont want to append but simply reuse an existing String object 
     *             and make sure we dont allocate more memory then necessary
     * @param   format     normal printf style format strings
     * @return  the number of characters that have been written, not counting the terminating null character.
     *          If an encoding error occurs or memory allocation failed, a negative number is returned.
     */
    int printf(const char* format, ...);

    //TEMP ADDED!! DEBUGGING
    //void dump();

    // constructors
    // creates a copy of the initial value.
    // if the initial value is null or invalid, or if memory allocation
    // fails, the string will be marked as invalid (i.e. "if (s)" will
    // be false).
    String(const char *cstr = "");
    String(const char *cstr, unsigned len);
    String(const String &str);
    String(const __FlashStringHelper *str) : String(reinterpret_cast<const char *>(str)) {};
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    String(String &&rval);
    String(StringSumHelper &&rval);
#endif
    explicit String(char c);
    explicit String(unsigned char, unsigned char base = 10);
    explicit String(int, unsigned char base = 10);
    explicit String(unsigned int, unsigned char base = 10);
    explicit String(long, unsigned char base = 10);
    explicit String(unsigned long, unsigned char base = 10);
    explicit String(float, unsigned char decimalPlaces = 2);
    explicit String(double, unsigned char decimalPlaces = 2);
    ~String(void);

    // memory management
    // return true on success, false on failure (in which case, the string
    // is left unchanged).  reserve(0), if successful, will validate an
    // invalid string (i.e., "if (s)" will be true afterwards)
    bool reserve(unsigned int size);
    char* resize(unsigned int size);

    inline unsigned int length(void) const
    {
        if(buffer) {
            return len;
        } else {
            return 0;
        }
    }
    bool empty() const {
        return !buffer || !len;
    }

    // creates a copy of the assigned value.  if the value is null or
    // invalid, or if the memory allocation fails, the string will be
    // marked as invalid ("if (s)" will be false).
    String & operator =(const String &rhs);
    String & operator =(const char *cstr);
    String & operator = (const __FlashStringHelper *str);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    String & operator =(String &&rval);
    String & operator =(StringSumHelper &&rval);
#endif

    // concatenate (works w/ built-in types)

    // returns true on success, false on failure (in which case, the string
    // is left unchanged).  if the argument is null or invalid, the
    // concatenation is considered unsucessful.
    unsigned char concat(const String &str);
    unsigned char concat(const char *cstr);
    unsigned char concat(char c);
    unsigned char concat(unsigned char c);
    unsigned char concat(int num);
    unsigned char concat(unsigned int num);
    unsigned char concat(long num);
    unsigned char concat(unsigned long num);
    unsigned char concat(float num);
    unsigned char concat(double num);
    unsigned char concat(const __FlashStringHelper * str);

    // if there's not enough memory for the concatenated value, the string
    // will be left unchanged (but this isn't signalled in any way)
    String & operator +=(const String &rhs)
    {
        concat(rhs);
        return (*this);
    }
    String & operator +=(const char *cstr)
    {
        concat(cstr);
        return (*this);
    }
    String & operator +=(char c)
    {
        concat(c);
        return (*this);
    }
    String & operator +=(unsigned char num)
    {
        concat(num);
        return (*this);
    }
    String & operator +=(int num)
    {
        concat(num);
        return (*this);
    }
    String & operator +=(unsigned int num)
    {
        concat(num);
        return (*this);
    }
    String & operator +=(long num)
    {
        concat(num);
        return (*this);
    }
    String & operator +=(unsigned long num)
    {
        concat(num);
        return (*this);
    }
    String & operator +=(float num)
    {
        concat(num);
        return (*this);
    }
    String & operator +=(double num)
    {
        concat(num);
        return (*this);
    }
    String & operator += (const __FlashStringHelper *str)
    {
        concat(str);
        return (*this);
    }

    friend StringSumHelper & operator +(const StringSumHelper &lhs, const String &rhs);
    friend StringSumHelper & operator +(const StringSumHelper &lhs, const char *cstr);
    friend StringSumHelper & operator +(const StringSumHelper &lhs, char c);
    friend StringSumHelper & operator +(const StringSumHelper &lhs, unsigned char num);
    friend StringSumHelper & operator +(const StringSumHelper &lhs, int num);
    friend StringSumHelper & operator +(const StringSumHelper &lhs, unsigned int num);
    friend StringSumHelper & operator +(const StringSumHelper &lhs, long num);
    friend StringSumHelper & operator +(const StringSumHelper &lhs, unsigned long num);
    friend StringSumHelper & operator +(const StringSumHelper &lhs, float num);
    friend StringSumHelper & operator +(const StringSumHelper &lhs, double num);
    friend StringSumHelper & operator +(const StringSumHelper &lhs, const __FlashStringHelper *rhs);

    // comparison (only works w/ Strings and "strings")
    operator StringIfHelperType() const
    {
        return buffer ? &String::StringIfHelper : 0;
    }
    int compareTo(const String &s) const;
    unsigned char equals(const String &s) const;
    unsigned char equals(const char *cstr) const;
    unsigned char operator ==(const String &rhs) const
    {
        return equals(rhs);
    }
    unsigned char operator ==(const char *cstr) const
    {
        return equals(cstr);
    }
    unsigned char operator !=(const String &rhs) const
    {
        return !equals(rhs);
    }
    unsigned char operator !=(const char *cstr) const
    {
        return !equals(cstr);
    }
    unsigned char operator <(const String &rhs) const;
    unsigned char operator >(const String &rhs) const;
    unsigned char operator <=(const String &rhs) const;
    unsigned char operator >=(const String &rhs) const;
    unsigned char equalsIgnoreCase(const String &s) const;
    unsigned char startsWith(const String &prefix) const;
    unsigned char startsWith(const String &prefix, unsigned int offset) const;
    unsigned char endsWith(const String &suffix) const;

    // character acccess
    char charAt(unsigned int index) const;
    void setCharAt(unsigned int index, char c);
    char operator [](unsigned int index) const;
    char& operator [](unsigned int index);
    void getBytes(unsigned char *buf, unsigned int bufsize, unsigned int index = 0) const;
    void toCharArray(char *buf, unsigned int bufsize, unsigned int index = 0) const
    {
        getBytes((unsigned char *) buf, bufsize, index);
    }
    const char * c_str() const
    {
        return buffer ? buffer : "";  //CHANGED!!!! DONT RETURN NULL
    }
    char *data() {
        return buffer;
    }

    // search
    int indexOf(char ch) const;
    int indexOf(char ch, unsigned int fromIndex) const;
    int indexOf(const String &str) const;
    int indexOf(const String &str, unsigned int fromIndex) const;
    int lastIndexOf(char ch) const;
    int lastIndexOf(char ch, unsigned int fromIndex) const;
    int lastIndexOf(const String &str) const;
    int lastIndexOf(const String &str, unsigned int fromIndex) const;
    String substring(unsigned int beginIndex) const
    {
        return substring(beginIndex, len);
    }
    ;
    String substring(unsigned int beginIndex, unsigned int endIndex) const;

    // modification
    void replace(char find, char replace);
    void replace(const String& find, const String& replace);
    void remove(unsigned int index);
    void remove(unsigned int index, unsigned int count);
    void toLowerCase(void);
    void toUpperCase(void);
    void trim(void);

    // parsing/conversion
    int toInt(void) const;
    long toLong(void) const;
    float toFloat(void) const;

    unsigned char concat(const char *cstr, unsigned int length);

protected:
    char *buffer;	        // the actual char array
    unsigned int capacity;  // the array length minus one (for the '\0')
    unsigned int len;       // the String length (not counting the '\0')
protected:
    void init(void);
    void invalidate(void);
    char* ensureCapacity(unsigned int size);

    // copy and move
    String & copy(const char *cstr, unsigned int length);
    String & copy(const __FlashStringHelper *pstr, unsigned int length);
#ifdef __GXX_EXPERIMENTAL_CXX0X__
    void move(String &rhs);
#endif
};

class StringSumHelper: public String
{
public:
    StringSumHelper(const String &s) :
        String(s)
    {
    }
    StringSumHelper(const char *p) :
        String(p)
    {
    }
    StringSumHelper(char c) :
        String(c)
    {
    }
    StringSumHelper(unsigned char num) :
        String(num)
    {
    }
    StringSumHelper(int num) :
        String(num)
    {
    }
    StringSumHelper(unsigned int num) :
        String(num)
    {
    }
    StringSumHelper(long num) :
        String(num)
    {
    }
    StringSumHelper(unsigned long num) :
        String(num)
    {
    }
    StringSumHelper(float num) :
        String(num)
    {
    }
    StringSumHelper(double num) :
        String(num)
    {
    }
};

inline String::String(const char *cstr)
{
    init();
    if(cstr) {
        copy(cstr, strlen(cstr));
    }
}

inline String::String(const char *cstr, unsigned len)
{
    init();
    if(cstr && len) {
        copy(cstr, len);
    }
}

inline String::String(const String &value)
{
    init();
    *this = value;
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
inline String::String(String &&rval)
{
    init();
    move(rval);
}

inline String::String(StringSumHelper &&rval)
{
    init();
    move(rval);
}
#endif

inline String::String(char c)
{
    init();
    char buf[2];
    buf[0] = c;
    buf[1] = 0;
    *this = buf;
}

inline String::String(unsigned char value, unsigned char base)
{
    init();
    char buf[1 + 8 * sizeof(unsigned char)];
    utoa(value, buf, base);
    *this = buf;
}

inline String::String(int value, unsigned char base)
{
    init();
    char buf[2 + 8 * sizeof(int)];
    itoa(value, buf, base);
    *this = buf;
}

inline String::String(unsigned int value, unsigned char base)
{
    init();
    char buf[1 + 8 * sizeof(unsigned int)];
    utoa(value, buf, base);
    *this = buf;
}

inline String::~String()
{
    if(buffer) {
        free(buffer);
    }
    init();
}

inline void String::init(void)
{
    buffer = NULL;
    capacity = 0;
    len = 0;
}

inline void String::invalidate(void)
{
    if(buffer) {
        free(buffer);
    }
    init();
}

inline bool String::reserve(unsigned int size)
{
    if (buffer && capacity >= size) {
        return true;
    }
    return !!ensureCapacity(size);
}

inline char* String::resize(unsigned int maxStrLen)
{
    char *newbuffer = ensureCapacity(maxStrLen);
    if (newbuffer) {
        len = maxStrLen;
    }
    return newbuffer;
}

inline char* String::ensureCapacity(unsigned int maxStrLen)
{
    size_t newSize = (maxStrLen + 16) & (~0xf);
    char *newbuffer = (char *)(buffer ? realloc(buffer, newSize) : malloc(newSize));
    if (newbuffer) {
        if (maxStrLen > len) {
            memset(newbuffer + len, 0, maxStrLen - len);
        }
        newbuffer[maxStrLen] = 0;
        capacity = newSize - 1;
    } else {
        if (buffer) {
            free(buffer);
        }
        capacity = len = 0;
    }
    buffer = newbuffer;
    return newbuffer;
}

inline String & String::copy(const char *cstr, unsigned int length)
{
    if(!reserve(length)) {
        invalidate();
        return *this;
    }
    len = length;
    memcpy(buffer, cstr, length);
    buffer[len] = 0;
    return *this;
}

inline String & String::copy(const __FlashStringHelper *pstr, unsigned int length)
{
    return copy(reinterpret_cast<const char *>(pstr), length);
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
inline void String::move(String &rhs)
{
    if(buffer) {
        if(capacity >= rhs.len) {
            strcpy(buffer, rhs.buffer);
            len = rhs.len;
            rhs.len = 0;
            return;
        } else {
            free(buffer);
        }
    }
    buffer = rhs.buffer;
    capacity = rhs.capacity;
    len = rhs.len;
    rhs.buffer = NULL;
    rhs.capacity = 0;
    rhs.len = 0;
}
#endif

inline String & String::operator =(const String &rhs)
{
    if(this == &rhs) {
        return *this;
    }

    if(rhs.buffer) {
        copy(rhs.buffer, rhs.len);
    } else {
        invalidate();
    }

    return *this;
}

#ifdef __GXX_EXPERIMENTAL_CXX0X__
inline String & String::operator =(String &&rval)
{
    if(this != &rval) {
        move(rval);
    }
    return *this;
}

inline String & String::operator =(StringSumHelper &&rval)
{
    if(this != &rval) {
        move(rval);
    }
    return *this;
}
#endif

inline String & String::operator =(const char *cstr)
{
    if(cstr) {
        copy(cstr, strlen(cstr));
    } else {
        invalidate();
    }

    return *this;
}

inline String & String::operator = (const __FlashStringHelper *pstr)
{
    if (pstr) copy(pstr, strlen_P((PGM_P)pstr));
    else invalidate();

    return *this;
}

inline unsigned char String::concat(const String &s)
{
    return concat(s.buffer, s.len);
}

inline unsigned char String::concat(const char *cstr, unsigned int length)
{
    unsigned int newlen = len + length;
    if(!cstr) {
        return 0;
    }
    if(length == 0) {
        return 1;
    }
    if(!reserve(newlen)) {
        return 0;
    }
    memcpy(buffer + len, cstr, length);
    buffer[newlen] = 0x00;
    len = newlen;

    return 1;
}

inline unsigned char String::concat(const char *cstr)
{
    if(!cstr) {
        return 0;
    }
    return concat(cstr, strlen(cstr));
}

inline unsigned char String::concat(char c)
{
    unsigned int newlen = ++len;
    if(!reserve(newlen)) {
        return 0;
    }
    buffer[newlen - 1] = c;
    buffer[newlen] = 0;

    return 1;
}

inline unsigned char String::concat(unsigned char num)
{
    char buf[1 + 3 * sizeof(unsigned char)];
    itoa(num, buf, 10);
    return concat(buf, strlen(buf));
}

inline unsigned char String::concat(int num)
{
    char buf[2 + 3 * sizeof(int)];
    itoa(num, buf, 10);
    return concat(buf, strlen(buf));
}

inline unsigned char String::concat(unsigned int num)
{
    char buf[1 + 3 * sizeof(unsigned int)];
    utoa(num, buf, 10);
    return concat(buf, strlen(buf));
}

inline unsigned char String::concat(long num)
{
    char buf[2 + 3 * sizeof(long)];
    ltoa(num, buf, 10);
    return concat(buf, strlen(buf));
}

inline unsigned char String::concat(unsigned long num)
{
    char buf[1 + 3 * sizeof(unsigned long)];
    ultoa(num, buf, 10);
    return concat(buf, strlen(buf));
}

inline unsigned char String::concat(float num)
{
    char buf[20];
    char* string = dtostrf(num, 4, 2, buf);
    return concat(string, strlen(string));
}

inline unsigned char String::concat(double num)
{
    char buf[20];
    char* string = dtostrf(num, 4, 2, buf);
    return concat(string, strlen(string));
}

inline unsigned char String::concat(const __FlashStringHelper * str)
{
    return concat(reinterpret_cast<const char *>(str));
}

inline int String::printf(const char* format, ...) {
    va_list args;
    va_start(args, format);
    int addLen = vsnprintf(NULL, 0, format, args);
    va_end(args);
    if (addLen < 0) { // exit in case of a formatting problem
        return addLen;
    }

    if (len) { // add to existing string, so create temp buffer
        String s;
        s.resize(addLen); // resize always allocates at least 1 byte more than addLen
        addLen = vsnprintf((char*)s.c_str(), addLen+1, format, args);
        concat(s);
    } else {  // printf into empty string, so simply reuse sting
        reserve(addLen + 1);
        addLen = vsnprintf((char*)c_str(), addLen+1, format, args);
        len = addLen;
    }
    return addLen; 
}

inline int String::compareTo(const String &s) const
{
    if(!buffer || !s.buffer) {
        if(s.buffer && s.len > 0) {
            return 0 - *(unsigned char *) s.buffer;
        }
        if(buffer && len > 0) {
            return *(unsigned char *) buffer;
        }
        return 0;
    }
    return strcmp(buffer, s.buffer);
}

inline unsigned char String::equals(const String &s2) const
{
    return (len == s2.len && compareTo(s2) == 0);
}

inline unsigned char String::equals(const char *cstr) const
{
    if(len == 0) {
        return (cstr == NULL || *cstr == 0);
    }
    if(cstr == NULL) {
        return buffer[0] == 0;
    }
    return strcmp(buffer, cstr) == 0;
}

inline unsigned char String::operator<(const String &rhs) const
{
    return compareTo(rhs) < 0;
}

inline unsigned char String::operator>(const String &rhs) const
{
    return compareTo(rhs) > 0;
}

inline unsigned char String::operator<=(const String &rhs) const
{
    return compareTo(rhs) <= 0;
}

inline unsigned char String::operator>=(const String &rhs) const
{
    return compareTo(rhs) >= 0;
}

inline unsigned char String::equalsIgnoreCase(const String &s2) const
{
    if(this == &s2) {
        return 1;
    }
    if(len != s2.len) {
        return 0;
    }
    if(len == 0) {
        return 1;
    }
    const char *p1 = buffer;
    const char *p2 = s2.buffer;
    while(*p1) {
        if(tolower(*p1++) != tolower(*p2++)) {
            return 0;
        }
    }
    return 1;
}

inline unsigned char String::startsWith(const String &s2) const
{
    if(len < s2.len) {
        return 0;
    }
    return startsWith(s2, 0);
}

inline unsigned char String::startsWith(const String &s2, unsigned int offset) const
{
    if(offset > len - s2.len || !buffer || !s2.buffer) {
        return 0;
    }
    return strncmp(&buffer[offset], s2.buffer, s2.len) == 0;
}

inline unsigned char String::endsWith(const String &s2) const
{
    if(len < s2.len || !buffer || !s2.buffer) {
        return 0;
    }
    return strcmp(&buffer[len - s2.len], s2.buffer) == 0;
}

inline char String::charAt(unsigned int loc) const
{
    return operator[](loc);
}

inline void String::setCharAt(unsigned int loc, char c)
{
    if(loc < len) {
        buffer[loc] = c;
    }
}

inline char & String::operator[](unsigned int index)
{
    static char dummy_writable_char;
    if(index >= len || !buffer) {
        dummy_writable_char = 0;
        return dummy_writable_char;
    }
    return buffer[index];
}

inline char String::operator[](unsigned int index) const
{
    if(index >= len || !buffer) {
        return 0;
    }
    return buffer[index];
}

inline int String::toInt(void) const
{
    if(buffer) {
        return atoi(buffer);
    }
    return 0;
}

inline long String::toLong(void) const
{
    if(buffer) {
        return atol(buffer);
    }
    return 0;
}

inline float String::toFloat(void) const
{
    if(buffer) {
        return atof(buffer);
    }
    return 0;
}

#endif  // __cplusplus
#endif  // String_class_h
