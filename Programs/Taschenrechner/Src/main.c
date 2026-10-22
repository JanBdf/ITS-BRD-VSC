/**
 ******************************************************************************
 * @file    main.c
 * @author  Lasse Boll
 * @brief   main für Taschenrechner anhand Aufgabe1
 ******************************************************************************
 */
#include "LCD_GUI.h"
#include "LCD_Touch.h"
#include "display.h"
#include "init.h"
#include "keypad.h"
#include "scanner.h"
#include "stackfunctions.h"
#include "terminal.h"
#include "token.h"
int main(void) {
   initITSboard(); // Initialisierung des ITS Boards

   GUI_init(DEFAULT_BRIGHTNESS); // Initialisierung des LCD Boards mit Touch
   TP_Init(false);               // Initialisierung des LCD Boards mit Touch

   init_display();
   Terminal term = makeKeyPad();

   while (1) {
      T_token token = next_token();
      T_Error err = dofunction(token, &term);
      if (err != 0) {
         clearTerm(&term);
         setTextColorTerm(RED, &term);
         printTerm((char*)errorText(err), &term);
         setTextColorTerm(BLACK, &term);
      }
   }
}
// EOF