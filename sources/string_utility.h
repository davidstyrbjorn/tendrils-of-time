#ifndef STRING_UTILITY_H
#define STRING_UTILITY_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

char* Substring(char *string, int position, int length)
{
   char *pointer;
   int c;
 
   pointer = malloc(length+1);
   
   if( pointer == NULL )
       exit(EXIT_FAILURE);
 
   for( c = 0 ; c < length ; c++ )
      *(pointer+c) = *((string+position-1)+c);      
       
   *(pointer+c) = '\0';
 
   return pointer;
}

char* InsertSubstring(char *dest, char *seed, int position)
{
    char *left, *right;
    int length = strlen(dest);

    left = Substring(dest, 1, position - 1 );      
    right = Substring(dest, position, length-position+1);

    dest = (char*)malloc(100);

    strcpy(dest, "");
    strcat(dest, left);
    free(left);

    strcat(dest, seed);

    strcat(dest, right);
    free(right);

    return dest;
}

#endif