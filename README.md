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

// Globalni promene:
// xmatej25/scanner.c
unsigned int LINE_NUM;
```

# Informace:
Pokud nastane neocekavana chyba (napriklad neprideleni pameti), volejte funkci void error_msg(int err_number, char type_of_fault), kde err_num bude indentifikateor, prideleny pomoci makra a type_of_fault rika, zda chyba ukonci program (volanim exit(err_num)) ci ne. Napriklad: <br>

```c
  char *temp; 
  temp = malloc(10);
  if (temp == NULL)
    error_msg(ERR_INTERPRET, 'f');
```

Funkce error_msg(err_num, 'f') ukonci program exitem s prislusnou navratovou hodnotou a vytiskne chybove hlaseni na stderr, tudiz pred jejim volanim uvolnete jakokouliv alokovoanou pamet! Nebojte se pouzit goto, usnadni vam velmi praci, ale pouzivejte ho s rozvahou. Pokud misto znak 'f' poslete do funkce 'w' (nebo cokoliv jineho), vytiskne se pouze chybove hlaseni a program se neukonci.
