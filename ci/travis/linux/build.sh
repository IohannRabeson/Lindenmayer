set -e
CC="gcc-7"
CXX="g++-7"

mkdir builds
cd builds
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="$TRAVIS_CONFIGURATION_NAME" -DCMAKE_PREFIX_PATH="/opt/qt510"
cmake --build ./ --clean-first --target all
