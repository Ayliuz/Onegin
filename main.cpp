#include <stdio.h>
#include <stdlib.h>
#include <clocale>

#define MAXNSTRING 1000
#define MAXTEXT 10000

int write_to_file( FILE* fileptr, char* text_table[], int n_table );
int copy_text ( FILE* txtptr, char* text_to, int* len);
int sort_in_table ( char file[], int length, char* table[], int* n_table );
int alphabet ( char** table, int n_table );

int main()
{
    printf( "~ Text sorting \n" );
    printf( "~      Zuev Ilya, 26.09.2018\n\n" );

    setlocale( LC_CTYPE, "rus");
    FILE *txtfile = fopen( "EOneg.txt" ,"r" );
    if ( txtfile == NULL )
    {
        printf( "Ошибка открытия файла для чтения\n" );
        return 0;
    }

    char* *text_table = (char**) calloc( MAXNSTRING , sizeof( *text_table ));
    int n_table = 0;

    char* text_memory = (char*) calloc( MAXTEXT , sizeof( *text_memory ));
    int len_text = 0;

    if ( !copy_text( txtfile, text_memory, &len_text))
    {
        printf( "Ошибка копирования текста\n" );
        return 0;
    }
    fclose( txtfile );

    if ( !sort_in_table( text_memory, len_text, text_table, &n_table ) )
    {
        printf( "Ошибка обработки текста\n" );
        return 0;
    }

    if ( !alphabet( text_table, n_table ) )
    {
        printf( "Ошибка сортировки\n" );
        return 0;
    }

    FILE *sort_txt = fopen( "SORToneg.txt", "w" );
    if ( txtfile == NULL )
    {
        printf( "Ошибка открытия файла для записи\n" );
        return 0;
    }

    if ( !write_to_file( sort_txt, text_table, n_table ))
    {
        printf( "Ошибка записи\n" );
        return 0;
    }

    fclose( sort_txt );
    free( text_table );
    free( text_memory );

    printf( "Проверьте файл: SORToneg.txt \n" );

    return 0;
}

//************************************
/// Writes a text to a file.
///
/// Parameters: [in] FILE* fileptr - file pointer\n
///             [in] char* text_table[] - a pointer to an array of text strings\n
///             [in] int n_table - length of text_table[] array\n
///
///
/// Output: 1 if writing was successful\n
///         0 if there is an error in program or data
//************************************

int write_to_file( FILE* fileptr, char* text_table[], int n_table )
{
    if ( fileptr == NULL || text_table == NULL )
    {
        return 0;
    }
    if ( n_table == 0 )
    {
        printf( "Пустой текст\n" );
        return 0;
    }

    for ( int str = 0; str <= n_table; ++str )
    {
        if ( fputs( text_table[str], fileptr ) == EOF )
        {
            return 0;
        }

        fputc( '\n', fileptr );
    }

    return 1;
}

//************************************
/// Copies the text from a file to program memory.
///
/// Parameters: [in] FILE* txtptr - file pointer\n
///             [in] char text_to[] - a pointer to an array of chars, where to copy the text\n
///             [out] int* len - length of text_to[] array\n
///
///
/// Output: 1 if copy was successful\n
///         0 if there is an error in program or data
//************************************


int copy_text ( FILE* txtptr, char text_to[], int* len)
{
    if ( txtptr == NULL || text_to == NULL  || len == NULL )
    {
        return 0;
    }

    int char_file = 0;
     while ( ( char_file = fgetc( txtptr ) ) != EOF )
    {
        text_to[(*len)++] = char_file;
        if ( (*len) >= MAXTEXT )
        {
            printf( "Слишком много букв\n" );
            return 0;
        }
    }
    text_to[++(*len)] = '\0';

    return 1;
}

//************************************
/// Puts pointers of strings from a text to an array
///
/// Parameters: [in] char file[] - a pointer to a text\n
///             [in] int length - a length of file[]\n
///             [in] char* table[] - a pointer to an array, where to put the string pointers\n
///             [out] int* n_table - a length of table[] array\n
///
///
/// Output: 1 if sorting was successful\n
///         0 if there is an error in program or data
//************************************

int sort_in_table ( char file[], int length, char* table[], int* n_table )
{
    if ( file == NULL || table == NULL || n_table == NULL )
    {
        return 0;
    }
    if ( length == 0 )
    {
        printf( "Пустой текст\n" );
        return 0;
    }

    table [*n_table] = file;
    for (   int f_ind = 0; f_ind < length ; ++f_ind )
    {
        if ( file[f_ind] == '\n' )
        {
            file[f_ind] = '\0';
            (*n_table)++;
            table [*n_table] = &(file[f_ind + 1]);
        }
    }
    file[length] = '\0';

    return 1;
}

//************************************
/// Sorts a number of strings in the alphabet order
///
/// Parameters: [in] char* table[] - a pointer to an array with string pointers\n
///             [in] int n_table - a length of table[] array\n
///
///
/// Output: 1 if sorting was successful\n
///         0 if there is an error in program or data
//************************************


int alphabet ( char** table, int n_table )
{
    if ( table == NULL )
    {
        return 0;
    }
    if ( n_table == 0 )
    {
        printf( "Пустой текст\n" );
        return 0;
    }

    for ( int sort_1 = 1; sort_1 <= n_table; ++sort_1 ) //сортировка вставками
    {
        char* swap_var = table[sort_1];
        int sort_2 = sort_1;
        while ( sort_2 > 0 && table[sort_2 - 1][0] > swap_var[0] )
        {
            table[sort_2] = table [sort_2 - 1];
            --sort_2;
        }
        table[sort_2] = swap_var;
    }

    return 1;
}
