/* 
 * OpenVirtualization: 
 * For additional details and support contact developer@sierraware.com.
 * Additional documentation can be found at www.openvirtualization.org
 * 
 * Copyright (C) 2011 SierraWare
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 *
 * color manager implementation
 */

#include <color_manager.h>
static u32 colors_32bit[MAX_COLOR_VALUES] = { 0xff000000, /* black */
	0xffffffff, /* white */
	0xffff0000, /* red */
	0xff00ff00, /* green */
	0xff0000ff, /* blue */
	0xff7f7f7f, /* grey */
	0xff000080, /* navy */
	0xff000040  /* dark_blue */
};

/**
 * @brief 
 *
 * This function returns the absolute value of the number
 *
 * @param number: Integer value for which the absolute value has to be computed
 *
 * @return: Returns the absolute value for the number
 */
int sw_abs(int number) 
{
  return (number >= 0) ? number : -number;
}

/*Func to calculate Contrast of the given color*/
/**
* @brief 
*
* @param input_color
*
* @return 
*/
u32 calc_contrast_color (u32 input_color){

	if(sw_abs(((input_color) & color(BLUE)) - color(NAVY)) <= 
			sw_abs(color(DARK_BLUE)) &&
			sw_abs(((input_color >>  8) & color(BLUE)) - color(NAVY)) <= 
			sw_abs(color(DARK_BLUE)) &&
			sw_abs(((input_color >> 16) & color(BLUE)) - color(NAVY)) <= 
			sw_abs(color(DARK_BLUE))) 
		return (color(GRAY) + input_color) & color(WHITE);

	else return input_color ^ color(WHITE);
}

/*Func to return color code based on the color name*/
/**
* @brief 
*
* @param col
*
* @return 
*/
u32 color(s32int col)
{
	if(col > MAX_COLOR_VALUES || col < 0)
		return -1;
	return colors_32bit[col];
}

/*Func to convert 32 bit color to 16 bit color*/
/**
* @brief 
*
* @param color
*
* @return 
*/
u16 bit32_to_bit16(u32 color)
{
	return ((((color >> 16) & 0xF8) << 8) | 
			(((color >> 8) & 0xFC) << 3) | 
			((color & 0xF8) >> 3));
}

/*Func to convert 24 bit color to 16 bit color*/
/**
* @brief 
*
* @param color
*
* @return 
*/
u16 bit24_to_bit16(u32 color)
{
	return ((((color >> 16) & 0xF8) << 8) | 
			(((color >> 8) & 0xFC) << 3) | 
			((color & 0xF8) >> 3));
}
