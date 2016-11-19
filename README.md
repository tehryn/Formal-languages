# Formal languages
# Project

Úkolem projektu je implementace interpretu jazyka IFJ16 pomocí jazyka C.

Jazykový interpret (anglicky) https://en.wikipedia.org/wiki/Interpreted_language <br>
Jazykový interpret (česky) https://cs.wikipedia.org/wiki/Interpret_(software) <br>

```c
// Makra:
// scanner.h
#define S_SIZE 16

// error.h
#define ERR_WARNING 0
#define ERR_LEXICAL_ANALYSIS 1
#define ERR_SYNTACTIC_ANALYSIS 2
#define ERR_SEM_NDEF_REDEF 3
#define ERR_SEM_COMPATIBILITY 4
#define ERR_SEM_OTHERS 6
#define ERR_INPUT_NUMBER 7
#define ERR_UNINICIALIZED_VAR 8
#define ERR_DIVISION_ZERO 9
#define ERR_OTHERS 10
#define ERR_INTERN_FAULT 99

// Globalni promene:
// scanner.c
unsigned int LINE_NUM;
char *SCANNER_WORD;

//enum
//scanner.h
enum {
S_BOOLEAN = 1, /*!< Keyword boolean */
S_BREAK, /*!< Keyword break */
S_CLASS, /*!< Keyword class */
S_CONTINUE, /*!< Keyword continue */
S_DO, /*!< Keyword do */
S_DOUBLE, /*!< Keyword double */
S_ELSE, /*!< Keyword else */
S_FALSE, /*!< Keyword false */
S_FOR, /*!< Keyword for */
S_IF, /*!< Keyword if */
S_INT, /*!< Keyword int */
S_RETURN, /*!< Keyword return */
S_STRING, /*!< Keyword String */
S_STATIC, /*!< Keyword static */
S_TRUE, /*!< Keyword true */
S_VOID, /*!< Keyword void */
S_WHILE, /*!< Keyword while */

TYPE_DOUBLE, /*!< data type double */
TYPE_INT, /*!< data type int */
TYPE_STRING, /*!< data type String */
TYPE_BOOLEAN, /*!< data type boolean */
TYPE_STATIC_DOUBLE, /*!< data type static double */
TYPE_STATIC_INT, /*!< data type static int */
TYPE_STATIC_STRING, /*!< data type static String */
TYPE_STATIC_BOOLEAN, /*!< data type static boolean */
// TODO [static]? void

BLOCK_COMMENT, /*!< identifikator of block comment */
LINE_COMMENT, /*!< identifikator of one line comment */

S_SIMPLE_IDENT, /*!< stands for simple identifikator */
S_FULL_IDENT, /*!< stands for full identifikator */

S_EQUAL, /*!< stands for == */
S_LESS_EQUAL, /*!< stands for <= */
S_GREATER_EQUAL, /*!< stands for >= */
S_NOT_EQUAL, /*!< stands for != */
S_LEFT_PARE, /*!< stands for ( */
S_RIGHT_PARE, /*!< stands for ) */
S_LEFT_BRACE, /*!< stands for { */
S_RIGHT_BRACE, /*!< stands for } */
S_COMMA, /*!< stands for , */
S_SEMICOMMA, /*!< stands for ; */
S_PLUS, /*!< stands for + */
S_MINUS, /*!< stands for - */
S_DIV, /*!< stands for / */
S_MUL, /*!< stands for * */
S_ASSIGNMENT, /*!< stands for = */
S_EOF /*!< stands for EOF */
};

//parser.h
enum {
  P_START = S_EOF + 1,
  P_CLASS,
  P_CLASS_BODY,
  P_TYPE,
  P_DEF,
  P_DEF_ARGUMENTS,
  P_DEF_ARGUMENTS2,
  P_IDENT,
  P_FUNC_BODY,
  P_VAR_EXPR,
  P_GUIDANCE,
  P_USE_ARGUMENTS,
  P_USE_ARGUMENTS2,
  P_RETURN_EXPR,
  P_ELSE_EXISTANCE,
  P_IF_ELSE_SECTION,
  P_FUNC_BODY_H1,
  P_FUNC_BODY_H2,
//  P_BOOL_TYPE,
  P_EXPR
};
```

# Informace:
Vzhledem k tomu, ze java (IFJ16 stoji na jave) ma vlastni spravu pameti a C ne, napsal jsem funkce pro spravu pameti. Funkce jsou primarne urceny pro vestavene funkce, kde by se meli pouzivat misto bezneho mallocu, reallocu a free, ale nic nam nebrani je pouzivat i jinde, znacne to usnadni praci s pameti. Aby jste videli, ze chovani je prakticky stejne, mate zde v kodu i puvodni funkce.
```c
//:
void *malloc(size_t size);
void *mem_alloc(size_t size, mem_list_t *L); // alokuje pamet, ulozi ukazatel na ni do seznamu L a vraci ukazatel na alokovanou pamet, stejne jako malloc

void *realloc(void *ptr, size_t size);
void *mem_realloc(void *ptr, size_t size, mem_list_t *L); // stejne jako realloc, akorat narozdil od nej pri neuspechu uvolni puvodne alokovanou pamet, tzn ze pokud alokace selze, uvolni i puvodni ukazatel

void free(void *ptr);
void free_memory(mem_list_t *L); // uvolni veskerou alokovanou pamet - veskere ukazatele v seznamu L

// mem_list_t *L inicializujte pred pouzitim touto funkci:
void mem_list_t_init(mem_list_t *L);
```

Pokud nastane neocekavana chyba (napriklad neprideleni pameti), volejte funkci void error_msg(int err_number, char type_of_fault), kde err_num bude indentifikateor, prideleny pomoci makra a type_of_fault rika, zda chyba ukonci program (volanim exit(err_num)) ci ne. Napriklad: <br>

```c
  char *temp;
  temp = malloc(10);
  if (temp == NULL)
    error_msg(ERR_INTERPRET, 'f');
```

Funkce error_msg(err_num, 'f') ukonci program exitem s prislusnou navratovou hodnotou a vytiskne chybove hlaseni na stderr, tudiz pred jejim volanim uvolnete jakokouliv alokovoanou pamet! Nebojte se pouzit goto, usnadni vam velmi praci, ale pouzivejte ho s rozvahou. Pokud misto znak 'f' poslete do funkce 'w' (nebo cokoliv jineho), vytiskne se pouze chybove hlaseni a program se neukonci.

# GIT:
Nastavení gitu: <br>
git config --global user.email "you@example.com" <br>
git config --global user.name "username" <br>
git config --global github.user "username" <br>
git config --global credential.helper cache (bude ukladat heslo) <br>
git config --global credential.helper "cache --timeout=3600" (heslo bude ulozeno hodinu namisto 15 minut) <br>
git clone https://github.com/tehryn/Formal-languages (stahne repozitar ze serveru do pocitace, vytvori slozku Formal-languages v agtualnim adresari) <br>
cd Formal-languages (veskere prikazy s praci nad soubory musi byt provadeny v gitove slozce projektu, jinak vam to bude nadavat) <br>

At uz vytvorite nebo upravite soubor a budete ho chtit nahrat na git, musite pouzit sekvenci nekolika prikazu. Jako prvni musite pridat soubory: <br>
git add filename.c <br>
nebo <br>
git add directory <br>
nebo muzete pridat vsechny soubory, ktere jste zmenili <br>
git add * <br>

Dale musite zmenu nejak komentovat: <br>
git commit (ale pozor, otevre se vam nano a bude to chtit doplnit komentare)<br>
nebo <br>
git commit -m "komentare..." <br>

a nakonec nahrat soubory na server: <br>
git push <br>

Abyste nahrali soubory, musite mit u sebe v pocitaci ale shodnou verzi projektu s githubem, tzn. pokud nekdo upravi nejaky soubor ve vasi branchi, musite jeho zmeny nahrat k sobe do pocitace, to provedete prikazem: <br>
git pull <br>
