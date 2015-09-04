/*----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#------  This File is Part Of : ----------------------------------------------------------------------------------------#
#------- _  -------------------  ______   _   --------------------------------------------------------------------------#
#------ | | ------------------- (_____ \ | |  --------------------------------------------------------------------------#
#------ | | ---  _   _   ____    _____) )| |  ____  _   _   ____   ____   ----------------------------------------------#
#------ | | --- | | | | / _  |  |  ____/ | | / _  || | | | / _  ) / ___)  ----------------------------------------------#
#------ | |_____| |_| |( ( | |  | |      | |( ( | || |_| |( (/ / | |  --------------------------------------------------#
#------ |_______)\____| \_||_|  |_|      |_| \_||_| \__  | \____)|_|  --------------------------------------------------#
#------------------------------------------------- (____/  -------------------------------------------------------------#
#------------------------   ______   _   -------------------------------------------------------------------------------#
#------------------------  (_____ \ | |  -------------------------------------------------------------------------------#
#------------------------   _____) )| | _   _   ___   ------------------------------------------------------------------#
#------------------------  |  ____/ | || | | | /___)  ------------------------------------------------------------------#
#------------------------  | |      | || |_| ||___ |  ------------------------------------------------------------------#
#------------------------  |_|      |_| \____|(___/   ------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Licensed under the GPL License --------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Copyright (c) Nanni <lpp.nanni@gmail.com> ---------------------------------------------------------------------------#
#- Copyright (c) Rinnegatamante <rinnegatamante@gmail.com> -------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Credits : -----------------------------------------------------------------------------------------------------------#
#-----------------------------------------------------------------------------------------------------------------------#
#- Smealum for ctrulib -------------------------------------------------------------------------------------------------#
#- StapleButter for debug font -----------------------------------------------------------------------------------------#
#- Lode Vandevenne for lodepng -----------------------------------------------------------------------------------------#
#- Sean Barrett for stb_truetype ---------------------------------------------------------------------------------------#
#- Special thanks to Aurelio for testing, bug-fixing and various help with codes and implementations -------------------#
#-----------------------------------------------------------------------------------------------------------------------*/
#include <psp2/ctrl.h>
#include <psp2/touch.h>
#include <psp2/display.h>
#include <psp2/gxm.h>
#include <psp2/types.h>
#include <psp2/moduleinfo.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/io/fcntl.h>
#include "include/luaplayer.h"
extern "C"{
	#include "include/utils.h"
	#include "include/draw.h"
}

static int lua_print(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 4) return luaL_error(L, "wrong number of arguments.");
	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);
	char* text = (char*)luaL_checkstring(L, 3);
	int color = luaL_checkinteger(L, 4);
	font_draw_string(x, y, RGBA8((color) & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF, (color >> 24) & 0xFF), text);
	return 0;
}

static int lua_pixel(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 3) return luaL_error(L, "wrong number of arguments.");
	int x = luaL_checkinteger(L, 1);
	int y = luaL_checkinteger(L, 2);
	int color = luaL_checkinteger(L, 4);
	draw_pixel(x, y, RGBA8((color) & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF, (color >> 24) & 0xFF));
	return 0;
}

static int lua_rect(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 5) return luaL_error(L, "wrong number of arguments.");
	int x1 = luaL_checkinteger(L,1);
	int x2 = luaL_checkinteger(L,2);
	int y1 = luaL_checkinteger(L,3);
	int y2 = luaL_checkinteger(L,4);
	int color = luaL_checkinteger(L,5);
	draw_rectangle(x1, y1, x1+x2, y1+y2, RGBA8((color) & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF, (color >> 24) & 0xFF));
	return 0;
}

static int lua_circle(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 4) return luaL_error(L, "wrong number of arguments.");
	int x = luaL_checkinteger(L,1);
	int y = luaL_checkinteger(L,2);
	int radius = luaL_checkinteger(L,3);
	int color = luaL_checkinteger(L,4);
	draw_circle(x, y, radius, RGBA8((color) & 0xFF, (color >> 8) & 0xFF, (color >> 16) & 0xFF, (color >> 24) & 0xFF));
	return 0;
}

static int lua_flip(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 0) return luaL_error(L, "wrong number of arguments.");
	swap_buffers();
	return 0;
}

static int lua_blank(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 0) return luaL_error(L, "wrong number of arguments.");
	sceDisplayWaitVblankStart();
	return 0;
}

static int lua_clear(lua_State *L)
{
    int argc = lua_gettop(L);
    if (argc != 0) return luaL_error(L, "wrong number of arguments.");
	clear_screen();
	return 0;
}

//Register our Controls Functions
static const luaL_Reg Screen_functions[] = {
  {"debugPrint",						lua_print},
  {"drawPixel",							lua_pixel},
  {"fillRect",							lua_rect},
  {"fillCircle",						lua_circle},
  {"clear",								lua_clear},
  {"flip",								lua_flip},
  {"waitVblankStart",					lua_blank},
  {0, 0}
};

void luaScreen_init(lua_State *L) {
	lua_newtable(L);
	luaL_setfuncs(L, Screen_functions, 0);
	lua_setglobal(L, "Screen");
}