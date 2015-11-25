//#define DEBUG_WITH_CONSOLE

#include "network.h"
#include "netdef.h"
#include "log.h"
#include "map.h"
#include "command.h"
#include "interface.h"

#ifdef _WIN32
#include "..\Common\TI_def.h"
#elif linux
#include "../Common/TI_def.h"
#endif