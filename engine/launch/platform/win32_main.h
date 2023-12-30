// Copyright (c) 2023 Berke Umut Biricik All Rights Reserved

#pragma once

#include <Windows.h>

#ifdef _DEBUG
int main(int argc, char* argv[]);
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
                   LPSTR lpCmdLine, int nShowCmd);
#endif
