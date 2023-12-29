// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#if _WIN32
#include "launch/platform/win32_main.h"
#elif __linux__
#include "launch/platform/linux_main.h"
#endif

#include "core/instance.h"
