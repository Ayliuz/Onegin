#include "D:\TX\TXlib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

//************************************
/// Get pointer to the end of the string excluding punctuation
///
/// Parameters: [in] const char* StrBegin - a pointer  to the beginning of string
///
/// Output: char* - pointer to the end of the string excluding punctuation
///
//************************************

char* end_seek_wout_puncts_russian(char* StrBegin)
{
    char* str_ptr = StrBegin + strlen(StrBegin);

    //while ( (*str_ptr < 'а') || (*str_ptr > 'я'))
    while( !isalpha(*str_ptr))                               //isalpha проверяет символ
    {
            str_ptr -= sizeof(*str_ptr);
    }

    return str_ptr;
}

//************************************
/// Compares two strings from their ends, ignoring punctuation in the end
///
/// Parameters: [in] const void* str1 - a pointer to the first string
///             [in] const void* str2 - a pointer to the second string\n
///
///
/// Output: positive value if the first string is greater than the second string\n
///         negative value if the second string is greater than the first string\n
///         zero value if the first string and the second string are equal
///
//************************************

int comp_str_inversed (const void* str1, const void* str2)
{
    char* cur1 = end_seek_wout_puncts_russian((char*)str1);
    char* cur2 = end_seek_wout_puncts_russian((char*)str2);

    while( (cur1 > ((char*) str1)) && (cur2 > ((char*) str2)) && (*cur1 == *cur2))
    {
        cur1 -= sizeof(*cur1);
        cur2 -= sizeof(*cur2);
    }

    if ( (*cur1) >  (*cur2))  return 1;
    if ( (*cur1) == (*cur2))  return 0;
    if ( (*cur1) <  (*cur2))  return -1;
}
int main()
{
    char* s1 = "Небрежный плод моих забав,";
    char* s2 = "Чтоб не измучилось дитя,";

    printf("%d\n", isalpha('я'));

    return 0;
}
