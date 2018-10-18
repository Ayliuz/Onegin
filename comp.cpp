#include <stdlib.h>
#include <stdio.h>
#include <clocale>


int strlen_end_seek_wout_puncts_russian(char** str_ptr)
{
    int len = 0;
    char* StrBegin = *str_ptr;
    while ( **str_ptr != '\0')
    {
        *str_ptr = StrBegin + (++len) * sizeof(**str_ptr);
    }

    while ( (**str_ptr < 'а') || (**str_ptr > 'я'))
    {
            *str_ptr -= sizeof(**str_ptr);
    }

    return len;
}

int comp_str_inversed (const void* str1, const void* str2)
{
    char* cur1 = *(char**)str1;
    char* cur2 = *(char**)str2;

    int len1 = strlen_end_seek_wout_puncts_russian(&cur1);

    int len2 = strlen_end_seek_wout_puncts_russian(&cur2);

    while( (cur1 > (*(char**) str1)) && (cur2 > (*(char**) str2)) && (*cur1 == *cur2))
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
    setlocale (LC_CTYPE, "rus");

    char* s1 = "Не мысля гордый свет забавить,";
    char* s2 = "Высоких дум и простоты;";

    int len1 = strlen_end_seek_wout_puncts_russian(&s1);
    int len2 = strlen_end_seek_wout_puncts_russian(&s2);
    printf("%d     %d\n", len1, len2);

    return 0;
}
