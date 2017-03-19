/*
    1 - получаем индексы для каждой из букв ключа
    2 - строим квадрат: общ. число букв / число букв в ключе
    3 -
  // прямой обход решетки
   cout << "0:" << endl;
   for (int i = 0; i < SIZE; i++)
      for (int j = 0; j < SIZE; j++)
         if (grid[i][j] == 1)
            cout << buf[i][j];
   cout << endl;

   // поворот решетки на 90 градусов по часовой стрелке
   cout << "90:" << endl;
   for (int i = 0; i < SIZE; i++)
      for (int j = 0; j < SIZE; j++)
         if (grid[SIZE-j-1][i] == 1)
            cout << buf[i][j];
   cout << endl;

   // поворот решетки на 180 градусов по часовой стрелке
   cout << "180:" << endl;
   for (int i = 0; i < SIZE; i++)
      for (int j = 0; j < SIZE; j++)
         if (grid[SIZE-i-1][SIZE-j-1] == 1)
            cout << buf[i][j];
   cout << endl;

   // поворот решетки на 270 градусов по часовой стрелке
   cout << "270:" << endl;
   for (int i = 0; i < SIZE; i++)
      for (int j = 0; j < SIZE; j++)
         if (grid[j][SIZE-i-1] == 1)
            cout << buf[i][j];
   cout << endl;
*/
#include <QCoreApplication>
#include <QString>
#include <QByteArray>
#include <iostream>
#include <fstream>
#include <QDebug>
#include <unistd.h>
#include <algorithm>
#include <qalgorithms.h>
#include <string>
#include <sys/stat.h>

#define SUCCESS 0
#define FAILURE 1

using namespace std;
static char *path_key;
static char *path_text;
static int  square_length;

static const QString get_keys(ifstream &file);
static const QString get_plain_text(const QString key,
                                   const QString encr_text);
static QString get_cube(QString encr_text, int colmn, int rows, int numb);
static const QString get_full_key(const QString key, int colmn, int row);
static int get_column_numb(const QString text);

static int menu(int argc, char **argv);
static void clear_space(QString &text);
static int get_row_numb(QString text);
static QString get_encr(ifstream &file);

int main(int argc, char *argv[]){
    cout << "\tHello. This is Kardano show!" << endl;
    cout << "\tNice to meet you! Let\'s start!" << endl;

    path_key = (char*)calloc(MAX_INPUT, sizeof(char*));    
    path_text = (char*)calloc(MAX_INPUT, sizeof(char*));

    // Get path to files
    if( menu(argc, argv) != 0 )
        return -FAILURE;

    ifstream opt_file(path_key);
    if( !opt_file.is_open()){
        fprintf(stderr, "Can't open file with"
                        " options: %s\r\n", strerror(errno));
        return -FAILURE;
    }

    ifstream encr_text(path_text);
    if( !encr_text.is_open()){
        fprintf(stderr, "Can't open file with"
                        " options: %s\r\n", strerror(errno));
        return -FAILURE;
    }

    const QString keys = get_keys(opt_file);
    const QString encr = get_encr(encr_text);

    int colmn = get_column_numb(keys);
    int rows = get_row_numb(keys);
    square_length = colmn * rows;      // Length of one square

    // Get encr text from each sub square
    QString frst_cube = get_cube(encr, colmn, rows, 0);
    QString scnd_cube = get_cube(encr, colmn, rows, 1);
    QString third_cube = get_cube(encr, colmn, rows, 2);
    QString fourth_cube = get_cube(encr, colmn, rows, 3);

    // Concatenate all sub squares
    QString concat_cube = frst_cube + scnd_cube + third_cube + fourth_cube;    

    // Get unfolded key and plain text
    QString unfld_key = get_full_key(keys, colmn, rows);
    QString plain_text = get_plain_text(unfld_key, concat_cube);

    cout << "\nYour key: " << endl;
    cout << "\t";
    for( int i = 0; i < keys.length(); i++ ){
        if( keys.at(i) == '\n' || keys.at(i) == '\r')
            cout << "\n\t";
        else
            cout << keys.toStdString().at(i);
    }
    cout << endl;
    cout << "Your encr phrase: " << encr.toStdString() << endl;
    cout << "Your plain text: " << plain_text.toStdString() << endl;
    cout << endl;

    return SUCCESS;
}

/**
 * @brief get_plain_text - Get decryptioned text;
 * @param key            - unfolded grid Cardano;
 * @param encr_text      - encryptioned text;
 * @return plain text;
 */
static const QString get_plain_text(const QString key, const QString encr_text){
    QString plain_text;
    int incr_row;
    int count_roll = 0;

    // Match the Cardano grid with the encryption text
    for( incr_row = 0; incr_row < encr_text.length(); incr_row++ ){
        if( QString::compare(key.at(incr_row), "1") == 0 )
            plain_text[count_roll++] = encr_text.at(incr_row);
    }

    return plain_text;
}

/**
 * @brief get_cube  - Obtaining individual squares of encrypted text;
 * @param encr_text - row of encrypted text;
 * @param colmn     - the number of columns in Cardano grid;
 * @param rows      - the number of rows in Cardano grid;
 * @param numb      - the ordinal number of the square;
 * @return String containing consecutive squares characters;
 */
static QString get_cube(QString encr_text, int colmn, int rows, int numb){
    QString parse_str;      
    int incr_row;
    int incr_colm;

    int incr = 0;
    int tmp_math = 2 * colmn;
    int half_square = square_length * 2; // Half from FULL square
    if( numb == 0 ){
        for( incr_row = 0; incr_row < rows; incr_row++ ){
            for( incr_colm = 0; incr_colm < colmn; incr_colm++ ){
                parse_str[incr++] = encr_text.at(incr_colm + incr_row * tmp_math);
            }
        }
    }
    else if( numb == 1 ){
        for( incr_row = 0; incr_row < rows; incr_row++ ){
            for( incr_colm = 0; incr_colm < colmn; incr_colm++ ){
                parse_str[incr++] = encr_text.at(incr_colm +
                                                 (tmp_math * incr_row + colmn));
            }
        }
    }
    else if( numb == 2 ){
        for( incr_row = 0; incr_row < rows; incr_row++ ){
            for( incr_colm = 0; incr_colm < colmn; incr_colm++ ){
                parse_str[incr++] = encr_text.at(incr_colm +
                                                 (tmp_math * incr_row + half_square + colmn));
            }
        }
    }
    else if( numb == 3 ){
        for( incr_row = 0; incr_row < rows; incr_row++ ){
            for( incr_colm = 0; incr_colm < colmn; incr_colm++ ){
                parse_str[incr++] = encr_text.at(incr_colm +
                                                 (tmp_math * incr_row + half_square));
            }
        }
    }
    else
        return nullptr;

    return parse_str;
}

/**
 * @brief get_full_key - We return a string consisting of
 *                          a Cardano lattice, turned for all squares;
 * @param key          -
 * @param colmn        - the number of columns in Cardano grid;
 * @param row          - the number of rows in Cardano grid;
 * @return a string consisting of a Cardano lattice, turned for all squares;
 */
static const QString get_full_key(const QString key, int colmn, int row){
    QString full_key;
    int incr = 0;    
    int i, j;

    for( i = 0; i < key.length(); i++ ){
        if( key.at(i) != '\n')
            full_key[incr++] = key.at(i);
    }

    // II square
    for( i = 0; i < row; i++ ){
        for( j = square_length - colmn; j >= 0; j -= colmn ){
            full_key[incr++] = full_key.at(j + i);
        }
    }

    // III square
    for( j = square_length - 1; j >= 0; j-- ){
        full_key[incr++] = full_key.at(j);
    }

    // IV square
    for( i = 0; i < row; i++ ){
        for( j = colmn - 1; j <= square_length - 1; j += colmn ){
            full_key[incr++] = full_key.at(j - i);
        }
    }

    return  full_key;
}

/**
 * @brief get_key - return key words or phrase for encryption, length of alphabet
 * @param file    - file which contains key words
 * @return s_key_words - if all was successfully, string of key phrase;
 *         NULL        - if was error;
 */
static const QString get_keys(ifstream &file){
    struct stat info_file;
    stat(path_key, &info_file);  // get size of file

    if( info_file.st_size <= 0 ){
        printf("Why your %s is empty? (^o^)\r\n", path_key);
        return NULL;
    }

    char *key_words = (char*)calloc(info_file.st_size, sizeof(char*));
    if( key_words == NULL ){
        printf("Couldn't allocate memory for key words. Sorry (X_X)\r\n");
        return NULL;
    }

    file.read(key_words, info_file.st_size);
    if( !file ) {
        printf("Couldn't read file with keys. Sorry (*-*)\r\n");
        return NULL;
    }

    QString s_key_words(key_words);
    get_row_numb(s_key_words);      // Else remove only '\n'

    free(key_words);

    return s_key_words;
}

/**
 * @brief get_encr - return string of text for encryption
 * @param file     - file which contains not encrypted text
 * @return s_text - if all was successfully, string of not encrypted text for encryption;
 *         NULL        - if was error;
 */
static QString get_encr(ifstream &file){
    struct stat info_file;
    stat(path_text, &info_file);

    if( info_file.st_size <= 0 ){   // Check size of file
        printf("Why your %s is empty? (^o^)\r\n", path_text);
        return NULL;
    }

    char *text = (char*) calloc(info_file.st_size, sizeof(char*));
    if( text == NULL ){     // If we can't allocate memory
        printf("Couldn't allocate memory for plaintext. Sorry (X_X)\r\n");
        return NULL;
    }

    file.seekg(0, ios_base::beg);           // Set cursor to start of file
    file.read(text, info_file.st_size);
    if( !file ){
        printf("Couldn't read file with plaintext. Sorry (*-*)\r\n");
        return NULL;
    }

    QString s_text(text);

    free(text);
    return s_text;
}

/**
 * @brief menu - Parse input arguments on keys and path to file;
 * @param argc - Count of input arguments;
 * @param argv - Input arguments;
 * @return SUCCESS - If all arguments is normal;
 *         FAILURE - If was error;
 */
static int menu(int argc, char **argv){
    string help = "\tYou must to use:\n"
                  "-k\t- file which contains key phrase;\n"                 
                  "-t\t- file which contains plaintext (not encrypted);\n"
                  "-h\t- this help view;\n";
    if( ( argc != 5 ) ){
        if( !strcmp(argv[1], "-h") ){
            cout << help << endl;
            return -FAILURE;
        }
        cout << "\tYou doing something wrong!" << endl;
        cout << help << endl;
        return -FAILURE;
    }

    int opt;
    while( (opt = getopt(argc, argv, "k:t:h:")) != -1 ){
        switch(opt){
        case 'k':
            cout << "\tYour key file will be: ";
            strcpy(path_key, optarg);
            cout << path_key << endl;
            break;
        case 't':
            cout << "\tYour plaintext file will be: ";
            strcpy(path_text, optarg);
            cout << path_text << endl;
            break;
        case 'h':
            cout << help << endl;
            return -FAILURE;
        default:
            cout << "\tYou wroooong! Why?" << endl;
            cout << help << endl;
            return -FAILURE;
        }
    }

    return SUCCESS;
}


/**
 * @brief get_row_numb - Clear string from \n symbols and count of number of rows;
 * @param text         - String of text (key, alphabet, plain text);
 */
static int get_row_numb(QString text){
    text = text.toLower();
    int pos = 0;
    int count_rows = 0;
    while( text.contains('\n') ) {
        pos = text.indexOf('\n');
        if( pos != -1 ){
            text.remove(pos, 1);
            count_rows++;
        }
    }

    return count_rows;
}

/**
 * @brief get_column_numb - Count of number of columns in Cardano grid;
 * @param text            - Cardano grid;
 * @return the number of columns in Cardano grid;
 */
static int get_column_numb(const QString text){
    int pos = 0;
    if( text.contains('\n') || text.contains('\r')) {
        pos = text.indexOf('\n');
        if( pos != -1 ){
            return pos;
        }
    }

    return pos;
}
