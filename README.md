# Formal languages
# Project

Úkolem projektu je implementace interpretu jazyka IFJ16 pomocí jazyka C.

Jazykový interpret (anglicky) https://en.wikipedia.org/wiki/Interpreted_language <br>
Jazykový interpret (česky) https://cs.wikipedia.org/wiki/Interpret_(software) <br>

Makra: <br>
xmatej25/scanner.h <br>
\#define BOOLEAN 1000 <br>
\#define BRAK 1001 <br>
\#define CLASS 1002 <br>
\#define CONTINUE 1003 <br>
\#define DO 1004 <br>
\#define DOUBLE 1005 <br>
\#define ELSE 1006 <br>
\#define FALSE 1007 <br>
\#define FOR 1008 <br>
\#define IF 1009 <br>
\#define INT 1010 <br>
\#define RETURN 1011 <br>
\#define STRING 1012 <br>
\#define STATIC 1013 <br>
\#define TRUE 1014 <br>
\#define VOID 1015 <br>
\#define WHILE 1016 <br>

\#define TYPE_DOUBLE 1100 <br>
\#define TYPE_INT 1101 <br>

Globalni promene: <br>
xmatej25/scanner.c <br>
unsigned LINE_NUM;

# Informace:
Pokud nastane neocekavana chyba (napriklad neprideleni pameti), volejte funkci void error(int err_id), kde err_id bude indentifikateor, prideleny pomoci makra. Napriklad: <br>

```c
  char *temp; 
  temp = malloc(10);
  if (temp == NULL)
    error(MEM_ALLOCATE_ERROR);
```

Funkce error() ukonci program exitem s prislusnou navratovou hodnotou a vytiskne chybove hlaseni na stderr, tudis pred jejim volanim uvolnete jakokouliv alokovoanou pamet! Nebojte se pouzit goto, usnadni vam velmi praci, ale pouzivejte ho s rozvahou.
