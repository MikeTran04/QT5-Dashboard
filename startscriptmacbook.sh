clear

qmake macbook.pro -spec macx-clang
make -j$(sysctl -n hw.ncpu)
make clean

# Start your application
./macbook
