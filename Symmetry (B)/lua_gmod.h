/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

#pragma once

// LUA_REGISTRYINDEX isn't the same as the one GMod uses
#define GMOD_REGINDEX 0xFFFFD8F0

// Types from garry's headers
enum
{
	TYPE_INVALID = -1,
	TYPE_NIL,
	TYPE_BOOL,
	TYPE_LIGHTUSERDATA,
	TYPE_NUMBER,
	_TYPE_STRING,
	TYPE_TABLE,
	TYPE_FUNCTION,
	TYPE_USERDATA,
	TYPE_THREAD,

	// UserData
	TYPE_ENTITY,
	TYPE_VECTOR,
	TYPE_ANGLE,
	TYPE_PHYSOBJ,
	TYPE_SAVE,
	TYPE_RESTORE,
	TYPE_DAMAGEINFO,
	TYPE_EFFECTDATA,
	TYPE_MOVEDATA,
	TYPE_RECIPIENTFILTER,
	TYPE_USERCMD,
	TYPE_SCRIPTEDVEHICLE,

	// Client Only
	TYPE_MATERIAL,
	TYPE_PANEL,
	TYPE_PARTICLE,
	TYPE_PARTICLEEMITTER,
	TYPE_TEXTURE,
	TYPE_USERMSG,

	TYPE_CONVAR,
	TYPE_IMESH,
	TYPE_MATRIX,
	TYPE_SOUND,
	TYPE_PIXELVISHANDLE,
	TYPE_DLIGHT,
	TYPE_VIDEO,
	_TYPE_FILE,

	TYPE_COUNT
};

// Type names for above
static const char* Name[] =
{
	"nil",
	"bool",
	"lightuserdata",
	"number",
	"string",
	"table",
	"function",
	"userdata",
	"thread",
	"entity",
	"vector",
	"angle",
	"physobj",
	"save",
	"restore",
	"damageinfo",
	"effectdata",
	"movedata",
	"recipientfilter",
	"usercmd",
	"vehicle",
	"material",
	"panel",
	"particle",
	"particleemitter",
	"texture",
	"usermsg",
	"convar",
	"mesh",
	"matrix",
	"sound",
	"pixelvishandle",
	"dlight",
	"video",
	"file",

	0
};

// Jewish structure for userdata because garry is a big fat jew
struct UserData
{
	void*			data;
	unsigned char	type;
};