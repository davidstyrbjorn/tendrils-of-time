#ifndef LSYSTEM_H
#define L_SYSTEM

#include<stdio.h>
#include<string.h>

#include"string_utility.h"

// L-System i want to implement, seperate the iterative step 
/*
Character        Meaning
   F	         Move forward by line length drawing a line
   f	         Move forward by line length without drawing a line
   +	         Turn left by turning angle
   -	         Turn right by turning angle
   |	         Reverse direction (ie: turn by 180 degrees)
   [	         Push current drawing state onto stack
   ]	         Pop current drawing state from the stack
   #	         Increment the line width by line width increment
   !	         Decrement the line width by line width increment
   @	         Draw a dot with line width radius
   {	         Open a polygon
   }	         Close a polygon and fill it with fill colour
   >	         Multiply the line length by the line length scale factor
   <	         Divide the line length by the line length scale factor
   &	         Swap the meaning of + and -
   (	         Decrement turning angle by turning angle increment
   )	         Increment turning angle by turning angle increment
*/

char* LSystemStart(char* axiom, int depth){
    size_t i = 0;
    char* new_current = (char*)malloc(1000000);
    strcpy(new_current, "");
    /* Stop looping when we reach the null-character. */
    while (axiom[i] != '\0') { 
        if(axiom[i] == 'F'){
            strcat(new_current, "F+F-F-FF+F+F-F");
        }
        else{
             strcat(new_current, &axiom[i]);
        }
        i++;
    }

    if(depth != 0)
        LSystemStart(new_current, --depth); 
    else
        return new_current;
}

void DrawLSystem(char* axiom, int line_length, int angle){

    rlPushMatrix();

    size_t i = 0;
    /* Stop looping when we reach the null-character. */
    while (axiom[i] != '\0') { 
        char c = axiom[i];
        switch(c){
            case 'F':
                DrawLineEx((Vector2){0.0, 0.0}, (Vector2){0.0, -line_length}, 2.0f, BLACK);
                rlTranslatef(0, -line_length, 0);
                break;
            case '+':
                rlRotatef(-angle, 0, 0, 1);
                break;
            case '-':
                rlRotatef(angle, 0, 0, 1);
                break;
        }
        i++;
    }

    rlPopMatrix();
}

#endif