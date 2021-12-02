#ifndef LSYSTEM_H
#define L_SYSTEM

#include<stdio.h>
#include<string.h>

#include"string_utility.h"

#define RL_MAX_MATRIX_STACK_SIZE 512
#include"rlgl.h"

// L-System i want to implement, seperate the iterative step 
/*
Character        Meaning
   F	         Move forward by line length drawing a line DONE
   f	         Move forward by line length without drawing a line
   +	         Turn left by turning angle DONE
   -	         Turn right by turning angle DONE
   |	         Reverse direction (ie: turn by 180 degrees)
   [	         Push current drawing state onto stack DONE
   ]	         Pop current drawing state from the stack DONE
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

char* LSystemStart(char* axiom, int depth, char* (*rule_func_ptr)(char)){
    size_t i = 0;
    char* new_current = (char*)malloc(1000000);

    strcpy(new_current, "");

    while (axiom[i] != '\0') { 
        char* replacement = rule_func_ptr(axiom[i]);
        if(replacement != NULL){
             strcat(new_current, rule_func_ptr(axiom[i]));
        }
        else{
            strncat(new_current, &axiom[i], 1);
        }
        i++;
    }


    if(depth != 0)
        LSystemStart(new_current, --depth, rule_func_ptr); 
    else
        return new_current;
}

void DrawLSystem(char* axiom, int line_length, int angle, float length_scale_factor){
    // Floating point length
    float length = (float)line_length;

    rlPushMatrix();

    size_t i = 0;
    /* Stop looping when we reach the null-character. */
    while (axiom[i] != '\0') { 
        char c = axiom[i];
        switch(c){
            // Rules table
            case 'F':
                DrawLineEx((Vector2){0.0, 0.0}, (Vector2){0.0, -length}, 4.0f, BLACK);
                rlTranslatef(0, -length, 0);
                break;
            case '+':
                rlRotatef(-angle, 0, 0, 1);
                break;
            case '-':
                rlRotatef(angle, 0, 0, 1);
                break;
            case ']': // Pop current drawing state from stack
                rlPopMatrix();
                break;
            case '[': // Save current drawing state on the stack
                rlPushMatrix();
                break;
            case '<':
                length /= length_scale_factor;
                break;
            case '>':
                length *= length_scale_factor;
                break;
        }
        i++;
    }

    rlPopMatrix();
}

#endif