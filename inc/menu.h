/*
 * menu.h
 *
 *  Created on: 10/11/2019
 */

#ifndef MENU_H_
#define MENU_H_

extern MenuItem menu[];
extern uint8_t	menuIndex;

void MenuUp();
void MenuDown();
void MenuSelect();

#endif /* MENU_H_ */
