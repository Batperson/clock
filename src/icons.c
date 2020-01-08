/*
 * icons.c
 *
 *  Created on: 7/01/2020
 */

#include "stm32f10x.h"
#include "macros.h"
#include "graphics.h"

const uint8_t bellData[] = {
	0x01, 0x00, //        #
	0x07, 0xC0, //      #####
	0x0F, 0xE0, //     #######
	0x0F, 0xE0, //     #######
	0x1F, 0xF0, //    #########
	0x1F, 0xF0, //    #########
	0x1F, 0xF0, //    #########
	0x1F, 0xF0, //    #########
	0x3F, 0xF8, //   ###########
	0x3F, 0xF8, //   ###########
	0x7F, 0xFC, //  #############
	0xFF, 0xFE, // ###############
	0xFF, 0xFE, // ###############
	0x07, 0x80, //      ####
	0x03, 0x00, //       ##
};

const uint8_t bellSlashData[] = {
	0x00, 0x82, //         #     #
	0x03, 0xE6, //       #####  ##
	0x07, 0xEC, //      ###### ##
	0x07, 0xD8, //      ##### ##
	0x0F, 0xB0, //     ##### ##
	0x0F, 0x68, //     #### ## #
	0x0E, 0xD8, //     ### ## ##
	0x0D, 0xB8, //     ## ## ###
	0x1B, 0x7C, //    ## ## #####
	0x16, 0xFC, //    # ## ######
	0x2D, 0xFE, //   # ## ########
	0x5B, 0xFF, //  # ## ##########
	0x37, 0xFF, //   ## ###########
	0x63, 0xC0, //  ##   ####
	0xC1, 0x80, // ##     ##
};

const uint8_t bellSnoozeData[] = {
	0x01, 0x00, //        #
	0x07, 0xC0, //      #####
	0x0F, 0xE0, //     #######
	0x0F, 0xE0, //     #######
	0x1F, 0xF0, //    #########
	0x1F, 0xF0, //    #########
	0x1F, 0x80, //    ######
	0x1F, 0xBE, //    ###### #####
	0x3F, 0x82, //   #######     #
	0x3F, 0xF4, //   ########## #
	0x7F, 0xEA, //  ########## # #
	0xFF, 0xD6, // ########## # ##
	0xFF, 0xA0, // ######### #
	0x07, 0xBE, //      #### #####
	0x02, 0x00, //       #
};

const Bitmap bellB = {
	16,
	16,
	Colour1Bpp,
	(void*)bellData
};

const Bitmap bellSlashB = {
	16,
	16,
	Colour1Bpp,
	(void*)bellSlashData
};

const Bitmap bellSnoozeB = {
	16,
	16,
	Colour1Bpp,
	(void*)bellSnoozeData
};

const Bitmap* bell 			= &bellB;
const Bitmap* bellSlash		= &bellSlashB;
const Bitmap* bellSnooze	= &bellSnoozeB;

