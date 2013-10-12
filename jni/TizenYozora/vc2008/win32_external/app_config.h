
#ifndef __APP_CONFIG_H__
#define __APP_CONFIG_H__

////////////////////////////////////////////////////////////////////////////////

#define APP_NAME  APP_YOZORA

////////////////////////////////////////////////////////////////////////////////

#define APP_DOKYU 1
#define APP_KANO  2
#define APP_AKIYU 3

////////////////////////////////////////////////////////////////////////////////

#if (APP_NAME == APP_YOZORA)
#	define MAIN_APP yozora
#	define TARGET_PIXEL unsigned long
#	define X_RESOLUTION 1280
#	define Y_RESOLUTION 720
#	define X_VIEWPORT   (X_RESOLUTION / 1)
#	define Y_VIEWPORT   (Y_RESOLUTION / 1)
#elif (APP_NAME == APP_DOKYU)
#	define MAIN_APP dokyu
#	define TARGET_PIXEL unsigned long
#	define X_RESOLUTION 640
#	define Y_RESOLUTION 400
#	define X_VIEWPORT   X_RESOLUTION
#	define Y_VIEWPORT   Y_RESOLUTION
#elif (APP_NAME == APP_KANO)
#	define MAIN_APP kano
#	define TARGET_PIXEL unsigned long
#	define X_RESOLUTION 640
#	define Y_RESOLUTION 400
#	define X_VIEWPORT   X_RESOLUTION
#	define Y_VIEWPORT   Y_RESOLUTION
#elif (APP_NAME == APP_AKIYU)
#	define MAIN_APP yunjr
#	define TARGET_PIXEL unsigned long
#	define X_RESOLUTION 720
#	define Y_RESOLUTION 1280
#	define X_VIEWPORT   (X_RESOLUTION *3/ 4)
#	define Y_VIEWPORT   (Y_RESOLUTION *3/ 4)
#else
#	error "APP_NAME not assigned"
#endif

#endif
