/**
 * \file   embedded_functions.h
 * \author Sava Nedeljkovic
 * \date   11.12.2016
 * \brief  Documentation for embeded functions
 */

/** @defgroup embedded_functions Embeded functions */
/** @addtogroup embedded_functions
  * @brief Embeded functions
  * of the language IFJ16.
  * @{
  */

#ifndef embedded_functions_h_INCLUDED
#define embedded_functions_h_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "garbage_collector.h"

/** 
 * \brief Function reads a string from STDIN.
 * \return array of chars (string) read from STDIN.
 */
char * readString();

/** 
 * \brief Function reads an integer value from STDIN.
 * \return integer value read from STDIN.
 */
int readInt();

/** 
 * \brief Function reads a number in double format from STDIN.
 * \return number in double format read from STDIN.
 */
double readDouble();

/** 
 * \brief Function prints string to STDOUT.
 */
void print(char * string);

/** @} */
#endif 	// embedded_functions_h_INCLUDED
