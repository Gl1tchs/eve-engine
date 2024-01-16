// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

namespace eve {

namespace script_glue {

/**
 * @brief Register entity component types to mono.
 * 
 */
void RegisterComponents();

/**
 * @brief Register interop functions between C# and C++
 * 
 */
void RegisterFunctions();

}  // namespace script_glue

}  // namespace eve