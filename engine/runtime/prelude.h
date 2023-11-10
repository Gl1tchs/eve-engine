// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#if defined(EVE_PLATFORM_WIN32)
#include "runtime/launch/win32_main.h"
#elif defined(EVE_PLATFORM_LINUX)
#include "runtime/launch/linux_main.h"
#endif

#include "core/instance.h"
