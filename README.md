# Formal languages
# Project

Úkolem projektu je implementace interpretu jazyka IFJ16 pomocí jazyka C.

Jazykový interpret (anglicky) https://en.wikipedia.org/wiki/Interpreted_language <br>
Jazykový interpret (česky) https://cs.wikipedia.org/wiki/Interpret_(software) <br>

```c
// Makra: 
// scanner.h 
#define S_BOOLEAN 1000
#define S_BREAK 1001
#define S_CLASS 1002
#define S_CONTINUE 1003
#define S_DO 1004
#define S_DOUBLE 1005
#define S_ELSE 1006 
#define S_FALSE 1007
#define S_FOR 1008
#define S_IF 1009
#define S_INT 1010
#define S_RETURN 1011
#define S_STRING 1012
#define S_STATIC 1013
#define S_TRUE 1014
#define S_VOID 1015
#define S_WHILE 1016

#define TYPE_DOUBLE 1100
#define TYPE_INT 1101

#define LINE_COMMENT 99998
#define BLOCK_COMMENT 99999

// error.h
#define ERR_LEXIKALNI_ANALYZA 1
#define ERR_SYNTAKTICKA_ANALYZA 2
#define ERR_SEM_NDEF_REDEF 3
#define ERR_SEM_KOMPATIBILITA 4
#define ERR_SEM_OSTATNI 6
#define ERR_VSTUP_CISLO 7
#define ERR_NEINIC_PROM 8
#define ERR_DELENI_NULOU 9
#define ERR_OSTATNI 10
#define ERR_INTERPRET 99

// main.c - makra budou v knihovne parseru pote
#define ERR_REACHED_MAX -100
#define MAX_LEN 1000

// Globalni promene:
// xmatej25/scanner.c
unsigned int LINE_NUM;
```

# Informace:
Vzhledem k tomu, ze java (IFJ16 stoji na jave) ma vlastni spravu pameti a C ne, napsal jsem funkce pro spravu pameti. Funkce jsou primarne urceny pro vestavene funkce, kde by se meli pouzivat misto bezneho mallocu, reallocu a free, ale nic nam nebrani je pouzivat i jinde, znacne to usnadni praci s pameti. Aby jste videli, ze chovani je prakticky stejne, mate zde v kodu i puvodni funkce.
```c
//:
void *malloc(size_t size);
void *mem_alloc(void *ptr, size_t size, mem_list_t *L); // alokuje pamet, ulozi ukazatel na ni do seznamu L a vraci ukazatel na alokovanou pamet, stejne jako malloc

void *realloc(void *ptr, size_t size);
void *mem_realloc(void *ptr, size_t size, mem_list_t *L); // stejne jako realloc, akorat narozdil od nej pri neuspechu uvolni puvodne alokovanou pamet, tzn ze pokud alokace selze, uvolni i puvodni ukazatel

void free(void *ptr);
void free_memory(mem_list_t *L); // uvolni veskerou alokovanou pamet - veskere ukazatele v seznamu L

// mem_list_t *L inicializujte pred pouzitim touto funkci:
mem_list_t * mem_list_t_init(mem_list_t *L);
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
