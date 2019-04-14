/*
 * touch_lib_link.h
 *
 *  Created on: Sep 15, 2018
 *      Author: willmitchell
 */

#ifndef TOUCH_LIB_LINK_H_
#define TOUCH_LIB_LINK_H_

#ifdef __cplusplus
extern "C" {
#endif

void (*touchStateChangeCallback)(void *);
void (*touchStateErrorCallback)(void *);
void * thisUI;

#ifdef __cplusplus
}
#endif



#endif /* TOUCH_LIB_LINK_H_ */
