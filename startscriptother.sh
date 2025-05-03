#!/bin/bash
clear

PORT=5000
PID=$(lsof -ti :$PORT)

if [ ! -z "$PID" ]; then
    echo "Port $PORT is in use by process $PID. Killing it..."
    kill -9 $PID
    sleep 1
fi

# Create a virtual environment if not exists
if [ ! -d "venv" ]; then
    python3 -m venv venv
fi

# Activate the virtual environment
source venv/bin/activate

# Install requirements
pip install --upgrade pip
pip install -r requirements.txt

# Start the Python server in the background
python src/py/youtube_server.py &

# Deactivate virtual environment
deactivate

# Navigate to the Qt project directory
# sudo apt-get install qt5-qmake qtbase5-dev qtcharts5-dev qtwebengine5-dev

# Build and start the Qt application
qmake qt5_dashboard.pro
make -j$(nproc)
make -j$(nproc)
make clean

# Set the environment variables
export QT_QPA_PLATFORM=xcb
export QTWEBENGINE_DISABLE_SANDBOX=1
export QTWEBENGINE_CHROMIUM_FLAGS="--no-sandbox --disable-gpu --disable-software-rasterizer --disable-dev-shm-usage --log-level=3"
export LIBGL_ALWAYS_SOFTWARE=1
export QT_QPA_EGLFS_INTEGRATION=eglfs_kms
export QT_QPA_EGLFS_KMS_NO_LIBINPUT=1
export QT_QPA_EGLFS_KMS_NO_MODESET=1
export QT_QPA_EGLFS_KMS_NO_DRM=1
export QT_QPA_EGLFS_KMS_NO_GBM=1
export QT_QPA_EGLFS_KMS_NO_FB=1
export QT_QPA_EGLFS_KMS_NO_WAYLAND=1



./qt5_dashboard &  # Run in the background

echo "Both youtube_server and qt5_dashboard are running."

