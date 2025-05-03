#!/bin/bash
clear

PORT=5000
PID=$(lsof -ti :$PORT)

if [ ! -z "$PID" ]; then
    echo "Port $PORT is in use by process $PID. Killing it..."
    kill -9 $PID
    sleep 1
fi
# Create a virtual environment if it doesn't exist
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

# Deactivate the virtual environment
deactivate

# Find the event number for the touch screen
TOUCHSCREEN_EVENT=$(ls /dev/input/event* | grep -m 1 'event')

if [ -z "$TOUCHSCREEN_EVENT" ]; then
    echo "Touch screen not found. Use Arrow keys to navigate."
else
    echo "Touch screen found at $TOUCHSCREEN_EVENT"
    export QT_QPA_GENERIC_PLUGINS=evdevtouch:$TOUCHSCREEN_EVENT
    export QT_QPA_EVDEV_TOUCHSCREEN_PARAMETERS=$TOUCHSCREEN_EVENT
fi

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




# (Optional) Tweak memory allocation behavior
export MALLOC_ARENA_MAX=2

# Build the Qt application
qmake pi_qt5_dashboard.pro
make -j$(nproc)
make clean

# Ensure the binary has the necessary capabilities
sudo setcap cap_net_bind_service,cap_net_raw+ep ./pi_qt5_dashboard

# Run the Qt5 dashboard as a normal user
./pi_qt5_dashboard --no-sandbox &

echo "Both youtube_server and pi_qt5_dashboard are running."
