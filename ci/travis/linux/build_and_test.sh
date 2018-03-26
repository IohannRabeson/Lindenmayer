set -e

TEMP_DIRECTORY="./temp_setup"
CMAKE_VERSION="3.9.3"

##############################################
#
# Setup CMake
#
##############################################

echo "Setup CMake $CMAKE_VERSION..."
sudo apt-get purge cmake

IFS='.'; read -ra CMAKE_VERSION_ARRAY <<< "$CMAKE_VERSION" ; unset IFS

CMAKE_VERSION_SHORT="${CMAKE_VERSION_ARRAY[0]}.${CMAKE_VERSION_ARRAY[1]}"

mkdir $TEMP_DIRECTORY
cd $TEMP_DIRECTORY
wget https://cmake.org/files/v$CMAKE_VERSION_SHORT/cmake-$CMAKE_VERSION.tar.gz
tar -xzf ./cmake-$CMAKE_VERSION.tar.gz
cd ./cmake-$CMAKE_VERSION/

# Do not output bootstrap log since it make the travis log too long to be displayed.
./bootstrap

sudo make -j4
sudo make install
cd -
rm -rf $TEMP_DIRECTORY

cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="$TRAVIS_CONFIGURATION_NAME"
cmake --build ./ --clean-first --target all

./tests/unit_tests

cd -
