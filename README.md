# Raspberry Pi Smart Dashboard

## :rocket: Prerequisites:

### :computer: Hardware
- **Raspberry Pi** (preferably with Raspbian OS) or any system with Qt5 installed.

### :package: Software
- **Qt5** (required for the GUI)
- **Python 3.x** (for running scripts)
- **Python Dependencies**: Install the required dependencies listed in the `requirements.txt` file.

### :warning: Warning:
- **First time building the project will take a long time (about 3-4 min) as the dependencies need to be installed before the project can run smoothly**.

## :clipboard: Important Notes:

### :file_folder: Key Folders and Files:
- **Makefile**: The Makefile for compiling the C++ source code.
- **Pi-start.sh**: Shell script for starting the Raspberry Pi dashboard.
- **docs/**: Contains project proposals, UML diagrams, and other project-related documents.
- **headers/**: Header files for the project's various components (e.g., `WebViewPage.h`, `dashboard.h`).
- **src/**: Source code files, with the `cpp/` folder containing C++ files, and `py/` for Python files.
- **resources/**: Contains images, icons, and other static resources for the dashboard.
- **requirements.txt**: Lists Python dependencies, including server-side components like `light_sensor.py` and `youtube_server.py`.

## :hammer_and_wrench: How to build the project:

### 1. On Raspberry Pi :point_up: :
- Run the startup script for the Raspberry Pi: ./Pi-start.sh

### 2. On macOS :apple: :
- For macOS, use the appropriate startup script: ./startscriptmacbook.sh

### 3. Other Systems :gear: :
- For other systems, use the generic startup script: ./startscriptother.sh

## :dart: Usage and Navigation:

### 1. On Raspberry Pi :point_up: :
- One swipe left or right to navigate between pages.
- Two swipes left will navigate to Youtube music. Three-finger left for Google Calendar.
- Touch to interact with buttons.

### 2. On MacOS or Other Systems(non-tablet) :desktop_computer: :
- Use left and right key to switch between pages.
- Interact with the buttons by clicking on it.


