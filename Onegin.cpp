#include <stdio.h>
#include <stdlib.h>
#include <clocale>
#include <string.h>
#include <ctype.h>
#include <assert.h>

const char READNAME[] = "EOneg.txt";
const char WRITENAME[] = "SORToneg.txt";

int string_counter (char* txtpointer);
char* read_from_file (const char *name);
int txt_file_length (FILE* txtpointer);
int write_to_file (const char* name, char** text_table, int n_table);
int add_to_file (const char* name, char** text_table, int n_table);
char** get_text_in_table (char* text);
int compare (const void* str1, const void* str2);
char** lexical_sorting (char** table, int n_table);
char** ends_lexical_sorting (char** table, int n_table);
int compare_inversed (const void* str1, const void* str2);
char* reverse_string_wout_puncts (char* str);
void free_table (char** table, int n_table);

int main()
{
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

    if ( !write_to_file(WRITENAME, text_lex_sorted, n_table)) {printf("Îøèáêà çàïèñè 1"); return 0;}
    if ( !add_to_file(WRITENAME, text_rythm_sorted, n_table)) {printf("Îøèáêà çàïèñè 2"); return 0;}
    if ( !add_to_file(WRITENAME, text_table, n_table))        {printf("Îøèáêà çàïèñè 3"); return 0;}

    free_table (text_table, n_table);
    free (full_text);

    printf ("Ïðîâåðüòå ôàéë: SORToneg.txt \n");

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

    qsort (table_lex_sorted, n_table, sizeof (*table), compare);
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

int compare (const void* str1, const void* str2)
{

    return strcmp (*(char**)str1, *(char**)str2);
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

    qsort (table_rythm_sorted, n_table, sizeof (*table), compare_inversed);
    table_rythm_sorted[n_table] = NULL;

    return table_rythm_sorted;
}

//************************************
/// Compares two strings from their ends, ignoring punctuation
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

int compare_inversed (const void* str1, const void* str2)
{
    char* buf1 = reverse_string_wout_puncts (*(char**) str1);
    char* buf2 = reverse_string_wout_puncts (*(char**) str2);

    int CompareResult = strcmp (buf1, buf2);
    free (buf1);
    free (buf2);

    return CompareResult;
}

//************************************
/// Inverts a string, making new string and ignoring punctuation in the end of strings
///
/// Parameters: [in] char* str - a pointer to the string\n
///
/// Output: pointer to reversed string
///
//************************************

char* reverse_string_wout_puncts (char* str)
{
    assert (str);

    int len = strlen (str);
    char* buf = (char*)  calloc (len + 1, sizeof (*buf));

    int buf_i = 0;
    for (int ch = len-1; ch >= 0; ch--)
    {
        if (!ispunct (str[ch])) buf[buf_i++] = str[ch];
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
