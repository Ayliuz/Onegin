#include "D:\TX\TXlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <clocale>
#include <string.h>
#include <ctype.h>
#include <assert.h>

const char READNAME[] = "EOneg.txt";
const char WRITENAME[] = "SORToneg.txt";

int string_counter (char*);
char* read_from_file (const char *);
int txt_file_length (FILE*);
int write_to_file (const char*, char**, int);
int add_to_file (const char*, char**, int);
char** get_text_in_table (char*);
int comp_str (const void*, const void*);
char** lexical_sorting (char**, int);
char** ends_lexical_sorting (char**, int);
int strlen_end_seek_wout_puncts_russian(char**);
int comp_str_inversed (const void*, const void*);
char* reverse_string_wout_puncts (char*);
void free_table (char**, int);

int main()
{

    $y;

    printf ("~ Text sorting \n");
    printf ("~ Zuev Ilya, 03.10.2018\n\n");

    setlocale (LC_CTYPE, "rus");

    char* full_text = NULL;
    if ( !(full_text = read_from_file(READNAME)))
    {
        printf ("Ошибка открытия файла для чтения\n");
        return 0;
    }

    int n_table = string_counter (full_text);

    char** text_table = get_text_in_table (full_text);
    char** text_lex_sorted = lexical_sorting(text_table, n_table);
    char** text_rythm_sorted = ends_lexical_sorting(text_table, n_table);

    if ( !write_to_file(WRITENAME, text_lex_sorted, n_table)) {printf("Ошибка записи 1"); return 0;}
    if ( !add_to_file(WRITENAME, text_rythm_sorted, n_table)) {printf("Ошибка записи 2"); return 0;}
    if ( !add_to_file(WRITENAME, text_table, n_table))        {printf("Ошибка записи 3"); return 0;}

    free_table (text_table, n_table);
    free (full_text);

    printf ("Проверьте файл: SORToneg.txt \n");

    return 0;
}

//************************************
/// Counts the number of strings in a text.
///
/// Parameters: [in] char* txtpointer - pointer to the text
///
///
/// Output: the number of strings
///
//************************************


int string_counter (char* txtpointer)
{
    assert (txtpointer != NULL);

    int str_n = 0;
    for (char* strptr = strchr (txtpointer, '\n'); strptr; strptr = strchr (strptr + 1, '\n'))
    {
        str_n++;
    }

    return (str_n + 1);
}

//************************************
/// Reads the text from a file to program memory.
///
/// Parameters: [in] char* name - name of the file to read from
///
///
/// Output: pointer to the text from the file in program memory
///
//************************************



char* read_from_file (const char *name)
{
    FILE *txtfile = fopen (name,"r");
    if (txtfile == NULL) return NULL;

    int len_text = txt_file_length (txtfile);
    char* text_memory = (char*) calloc (len_text + 1, sizeof (*text_memory));

    if (fread (text_memory, sizeof (*text_memory), len_text, txtfile) <= 0) return NULL;

    fclose (txtfile);
    return text_memory;
}

//************************************
/// Gives a text file's length.
///
/// Parameters: [in] FILE* txtpointer - file pointer\n
///
///
///
/// Output: length of the file\n
///
//************************************

int txt_file_length (FILE* txtpointer)
{
    fseek (txtpointer, 0, SEEK_END);
    int buf_len = ftell (txtpointer);
    rewind (txtpointer);
    return buf_len;
}

//************************************
/// Writes a text to a file.
///
/// Parameters: [in] char* name - name of the file to write
///             [in] char** text_table - a pointer to an array of text strings\n
///             [in] int n_table - length of text_table[] array\n
///
///
/// Output: 1 if writing was successful\n
///
//************************************

int write_to_file (const char* name, char** text_table, int n_table)
{
    FILE *sort_txt = fopen (name, "w");
    assert (sort_txt && text_table);

    for (int str = 0; str < n_table; ++str)
    {
        if (fputs (text_table[str], sort_txt) == EOF) return 0;
        fputc ('\n', sort_txt);
    }
    fputc ('\n', sort_txt);

    fclose (sort_txt);
    return 1;
}

//************************************
/// Adds a text to a file.
///
/// Parameters: [in] char* name - name of the file to write
///             [in] char** text_table - a pointer to an array of text strings\n
///             [in] int n_table - length of text_table[] array\n
///
///
/// Output: 1 if writing was successful\n
///
//************************************


int add_to_file (const char* name, char** text_table, int n_table)
{
    FILE *sort_txt = fopen (name, "a");
    assert (sort_txt && text_table);

    for (int str = 0; str < n_table; ++str)
    {
        if (fputs (text_table[str], sort_txt) == EOF) return 0;
        fputc ('\n', sort_txt);
    }
    fputc ('\n', sort_txt);

    fclose (sort_txt);
    return 1;
}

//************************************
/// Puts pointers of strings from a text to an array
///
/// Parameters: [in] char* text - a pointer to the text\n
///
/// Output: pointer to the array of string pointers
///
//************************************

char** get_text_in_table (char* text)
{
    assert(text);

    char* *table= (char**) calloc (string_counter (text) + 1, sizeof (*table));
    table[0] = text;

    int index = 1;
    for (char* strptr = strchr (text, '\n'); strptr; strptr = strchr (strptr + 1, '\n'))
    {
        *strptr = '\0';
        table[index++] = strptr + 1;
    }
    table[index] = NULL;

    return table;
}

//************************************
/// Sorts a number of strings in the alphabet order and puts them in a new array
///
/// Parameters: [in] char* *table - a pointer to an array with string pointers\n
///             [in] int n_table - a length of table[] array\n
///
///
/// Output: char** - pointer to the array of string pointers
///
//************************************


char** lexical_sorting (char** table, int n_table)
{
    assert(table);

    char** table_lex_sorted = (char**) calloc (n_table + 1, sizeof (*table));
    for (int i = 0; i < n_table; ++i)
    {
        table_lex_sorted[i] = table [i];
    }

    qsort (table_lex_sorted, n_table, sizeof (*table), comp_str);
    table_lex_sorted[n_table] = NULL;

    return table_lex_sorted;
}

//************************************
/// Compares two strings
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

int comp_str (const void* str1, const void* str2)
{

    return strcmp ( *(char**)str1, *(char**)str2);
}

//************************************
/// Sorts a number of strings in the alphabet order from their ends and puts them in a new array
///
/// Parameters: [in] char* *table - a pointer to an array with string pointers\n
///             [in] int n_table - a length of table[] array\n
///
///
/// Output: char** - pointer to the array of string pointers
///
//************************************

char** ends_lexical_sorting (char** table, int n_table)
{
    assert(table);

    char** table_rythm_sorted = (char**) calloc (n_table + 1, sizeof (*table));
    for (int i = 0; i < n_table; ++i)
    {
        table_rythm_sorted[i] = table [i];
    }

    qsort (table_rythm_sorted, n_table, sizeof (*table), comp_str_inversed);
    table_rythm_sorted[n_table] = NULL;

    return table_rythm_sorted;
}

//************************************
/// Counts length of string and points to the end of the string excluding punctuation
///
/// Parameters: [in] char** str_ptr - a pointer to the pointer to the beginning of string
///
/// Output: int len - length of the string, change str_ptr to the end of the string excluding punctuation
///
//************************************

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



//************************************
/// Inverts a string, making new string and ignoring punctuation in the end of strings
///
/// Parameters: [in] char* str - a pointer to the string\n
///
/// Output: pointer to reversed string
///
//************************************

char* reverse_string_wout_puncts (char* str) //полезная функция, но уже не используется в программе
{
    assert (str);

    int len = strlen (str);
    char* buf = (char*)  calloc (len + 1, sizeof (*buf));

    int buf_i = 0;
    for (int ch = len-1; ch >= 0; ch--)
    {
        if ( !ispunct (str[ch])) buf[buf_i++] = str[ch];
    }

    return buf;
}

//************************************
/// Frees text's and string pointers' memory
///
/// Parameters: [in] char* *table - a pointer to an array with string pointers\n
///             [in] int n_table - a length of table[] array\n
///
/// Output: void
///
//************************************


void free_table (char** table, int n_table)
{
    for (int i = 0; i < n_table; i++)
    {
        memset (table[i], '\0', strlen(table[i]));
        free (table[i]);
    }
}
