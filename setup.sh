#!/bin/bash

# Create extern directory if it doesn't exist
mkdir -p extern
cd extern

# Install vcpkg
if [ ! -d "vcpkg" ]; then
    echo "Cloning vcpkg..."
    git clone https://github.com/Microsoft/vcpkg.git
    cd vcpkg
    ./bootstrap-vcpkg.sh
    cd ..
fi

# Create vcpkg.json in the project root
cat > ../vcpkg.json << 'EOF'
{
    "name": "my-project",
    "version": "1.0.0",
    "dependencies": [
        {
            "name": "openscenegraph",
            "version>=": "3.6.5"
        },
        {
            "name": "osgearth",
            "version>=": "3.4"
        },
        "gdal",
        "curl",
        "sqlite3",
        "tinyxml",
        "libzip"
    ],
    "builtin-baseline": "2024-01-20",
    "overrides": [
        {
            "name": "openscenegraph",
            "version": "3.6.5"
        },
        {
            "name": "osgearth",
            "version": "3.4"
        }
    ]
}
EOF

# Export vcpkg root environment variable
echo "export VCPKG_ROOT=$(pwd)/vcpkg" >> ~/.bashrc
source ~/.bashrc

# Build Cesium Native
if [ ! -d "cesium-native" ]; then
    echo "Cloning and building cesium-native..."
    git clone --recursive https://github.com/CesiumGS/cesium-native.git
    cd cesium-native
    git checkout v0.37.0
    git submodule update --init --recursive
    mkdir build
    cd build
    cmake .. -DCMAKE_BUILD_TYPE=Release \
             -DCMAKE_INSTALL_PREFIX=../install \
             -DCESIUM_TRACING_ENABLED=OFF
    make -j$(nproc)
    make install
    cd ../..
fi

cd ..

echo "Setup complete!"
echo "vcpkg and Cesium Native have been installed and built"
echo "Note: You may need to run 'source ~/.bashrc' in your current terminal"