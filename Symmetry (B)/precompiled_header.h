/**                  ====================                      *
  *      _____                                __               *
  *     / ___/__  ______ ___  ____ ___  ___  / /________  __   *
  *     \__ \/ / / / __ `__ \/ __ `__ \/ _ \/ __/ ___/ / / /   *
  *    ___/ / /_/ / / / / / / / / / / /  __/ /_/ /  / /_/ /    *
  *   /____/\__, /_/ /_/ /_/_/ /_/ /_/\___/\__/_/   \__, /     *
  *        /____/                                  /____/      *
  *   - by Defc0n                                              *
  *                  ====================                      **/

// PRECOMPILED HEADER FILE

// == Include Symmetry-Lib precompiled header
#include "lib_precompiled_header.h"

// == Include SDK precompiled header
#include "src_generic.h"

// == Custom Entity Class
#include "src_entity.h"

// == Include other SDK files
#include "steam/isteamfriends.h"
#include "inetchannelinfo.h"
#include "inetchannel.h"
#include "inetchannelinfo.h"
#include "prediction.h"
#include "tier2\tier2.h"
#include "cdll_int.h"
#include "engine\ivmodelrender.h"
#include "engine\ivmodelinfo.h"
#include "icliententitylist.h"
#include "steam\isteamclient.h"
#include "engine\ienginetrace.h"
#include "eiface.h"
#include "toolframework\ienginetool.h"
#include "playerinfomanager.h"
#include "input.h"
#include "clientmode_shared.h"
#include "igamemovement.h"
#include "iprediction.h"
#include "vgui\ipanel.h"

// == Include some Symmetry-Lib files that are NOT included in it's precompiled header
#include "mem.h"
#include "stream.h"
#include "file.h"

// Detours
#include "vmt.h"

// Net
#include "net.h"
#include "netmsg_base.h"
#include "netmsg_ipc_addcommand.h"
#include "netmsg_ipc_remcommand.h"
#include "netmsg_ipc_exit.h"
#include "netmsg_ipc_print.h"
#include "netmsg_ipc_clear.h"
#include "apipe_client.h"

// == Include external dependancies
#include "MinHook.h"

// Lua
#include "lua.hpp"
#include "lobject.h"
#include "lstate.h"

// Frequently included files
#include "console.h"