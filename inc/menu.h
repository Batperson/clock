/*
 * menu.h
 *
 *  Created on: 10/11/2019
 */

#ifndef MENU_H_
#define MENU_H_

#define MENU_TIMEOUT_MILLIS			1500
#define MAX_RENDER_MENU_ITEMS		12

typedef void (*PMenuProc)(uint32_t);

typedef struct {
	char* 					text;
	void*					proc;
	uint32_t				arg;
} MenuItem, *PMenuItem;

void MenuUp();
void MenuDown();
void MenuSelect();
void RenderMenu();
void SetCurrentMenu(PMenuItem);

#endif /* MENU_H_ */
