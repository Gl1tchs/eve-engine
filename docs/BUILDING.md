<!--
 Copyright (c) 2023 Berke Umut Biricik All Rights Reserved
-->

# Building Instructions

These instructions guide you through the build process of the CMake project. After building, it's necessary to set specific environment variables in order for the project to function correctly.

## Build Instructions

1. Make sure you have CMake installed on your system. If not, install it from [CMake's official website](https://cmake.org/).

2. Clone the repository or navigate to your project directory.

    ```bash
    git clone https://github.com/Gl1tchs/eve-engine.git --recursive
    cd eve-engine
    ```

3. Create a build directory and navigate to it.

    ```bash
    mkdir build
    cd build
    ```

4. Configure the build using CMake.

    ```bash
    cmake ..
    ```

5. Build the project.

    ```bash
    cmake --build .
    ```

## Setting Environment Variables

After successfully building the project, it's important to set the following environment variable to the system:

### EVE_ENGINE_DIR

Set `EVE_ENGINE_DIR` to the directory where the built engine resides. This might be the `build` directory or a specified installation directory.

For example:

- If the built engine is in the `build` directory of your project:

    ```bash
    export EVE_ENGINE_DIR=/path/to/engine/project/build
    ```

- If the built engine is installed in a different directory:

    ```bash
    export EVE_ENGINE_DIR=/path/to/engine/installation/directory
    ```

Remember to adjust the paths based on your specific project structure and the location where your built artifacts and header files are located.

It's recommended to add these environment variable settings to your system's configuration file (e.g., .bashrc, .bash_profile, .zshrc, etc.) to persist across sessions.

Once the environment variables are set, your project should be ready to run using the configured settings.

Good luck!