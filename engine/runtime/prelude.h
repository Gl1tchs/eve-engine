// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#ifndef EVE_RUNTIME_PRELUDE_H_
#define EVE_RUNTIME_PRELUDE_H_

#if defined(EVE_PLATFORM_WIN32) 
#include "runtime/launch/win32_main.h"
#elif defined(EVE_PLATFORM_LINUX)
#include "runtime/launch/linux_main.h"
#endif

#include "runtime/launch/guarded_main.h"

#endif  // EVE_RUNTIME_PRELUDE_H_