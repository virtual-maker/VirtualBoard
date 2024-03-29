# VirtualBoard Extension for Visual Studio

![VirtualBoard Logo](./doc/images/Logo-VirtualBoard_128.png)

Build and debug Arduino sketches with Visual C++ (MSVC) and Visual Studio 2022.

> I can't promise that you will get your Arduino project  
> running 5 times faster with this extension.  
> But I am sure that you will have 5 times more fun with it.

Immo Wache - Inventor of VirtualBoard

![VS2022 with VirualBoard in debug mode](./doc/images/02-vs2022-with-virtualboard.png)

## Overview

Use the Visual Studio integrated development environment (IDE) to develop, build, run and debug your Arduino sketches as an executable directly at your Windows computer. Use network and serial connections of your computer directly from your Arduino sketch. Connect to I/O, SPI and I2C pins via [IO-Warrior](https://www.codemercs.com/en/io) hardware, connected via USB.

## Advantages

- Make up use of the super fast MSVC compiler and comprehensive Visual Studio debug features to speed up your sketch code development,
- Get a project template to obtain all required MSVC settings preconfigured for your sketch,
- Use the Arduino compatible VirtualBoard NuGet package, which provides the standard Arduino functions and libraries for your Windows sketch executable,
- Keep your sketch folder compatible with Arduino IDE or Arduino CLI and upload your sketches to real Arduino compatible boards,
- Add and use MSVC compatible Arduino libraries to your sketches or use special MSVC compatible library versions,
- Connect your running sketch simulation at your Windows computer with real I/O, SPI and I2C pins via [IO-Warrior](https://www.codemercs.com/en/io) hardware.

## Limitations

- MSVC, used by Visual Studio differs in some C++ language features from GCC compiler, used by the Arduino IDE,
- The Windows executable uses the 32bit features of your computer CPU which differs e.g. from the 8bit Arduino Uno,
- The Windows executable also uses huge memory resources of your computer, compared to the 2K RAM size of an Arduino Uno,
- Windows is not a real time OS, so no interrupts or time critical response time is supported.

## Getting Started

### Prerequisites

To create VirtualBoard applications for your Arduino sketches you will need [Visual Studio 2022](https://visualstudio.microsoft.com/vs/) with workload "Desktop development with C++" installed.  
![VS2022 Installer C++ workload](./doc/images/01-vs2022-workload-cpp.png)

### Install Visual Studio extension
1. Launch Visual Studio 2022, then click `Continue without code`. 
   ![Select "Continue without code"](./doc/images/03-start-vs-without-code.png)
   
2. Click `Extensions -> Manage Extensions` from the Menu Bar.
   ![Select "ManageExtensions" ](./doc/images/04-manage-extensions.png)
   
3. In the Extension Manager expand the **Online** node and search for `Arduino`, install the `VirtualBoardExtension` or download it from the [Visual Studio Marketplace](https://marketplace.visualstudio.com/items?itemName=virtual-maker.virtualboardextension).
   ![Download VirtualBoard extension](./doc/images/05-install-virtualboard-extension.png)
   
4. Close Visual Studio to actual start the installation of the extension. Then restart Visual Studio again.

### Create an Arduino sketch application from template
1. Launch Visual Studio 2022, then click `Create a new project` to start the project creation wizard.
   ![Select "Create a new project"](./doc/images/06-vs-create-new-project.png)
   
2. In the "Create a new project" wizard step search for `Arduino` and select the `ArduinoProjectTemplate` by double click at template or press button `Next`.
   ![Select "ArduinoProjectTemplate"](./doc/images/07-vs-select-arduino-project-template.png)

3. In last wizard step "Configure your new project" enter your project name.
   This will also be your Arduino sketch name and project folder name. Then press button `Create`.
   ![Configure new project settings](./doc/images/08-vs-configure-project-settings.png)

4. Now Visual Studio opens your new created VirtualBoard project for Arduino. In the solution explorer tree find your `sketch.h` and Arduino `*.ino` sketch file.
   ![Your project in Visual Studio IDE](./doc/images/09-vs-ide-project-view.png)

5. Also take a look at the `Application.cpp` file. This file is the main entry point of your Visual C++ project. All Arduino core libraries and your sketch file are included from here.
   ![Project entry file Application.cpp](./doc/images/10-project-entry-file-application.png)

6. Now you can start to write and debug your Arduino sketch code - Happy coding!



## Videos
- [VirtualBoard Installation](https://vimeo.com/727126315) - Install VS2022 Community Edition and the VirtualBoard extension
- [VirtualBoard Sketch Creation](https://vimeo.com/727133698) - Use the Arduino project template to create a new or already existing Arduino sketch
- Debug a VirtualBoard sketch with VS2022
- Add additional Arduino libraries
- Connect to real I/O, SPI and I2C pins via IO-Warrior hardware