// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#if _WIN32
#include "runtime/launch/win32_main.h"
#elif __linux__
#include "runtime/launch/linux_main.h"
#endif

#include "core/instance.h"
