set -e

# install more recent Qt 5.
sudo apt-add-repository --yes ppa:beineri/opt-qt-5.10.1-trusty
sudo apt-get -qq update
sudo apt-get --yes install qt510-meta-minimal qt510tools qt510location
sudo /opt/qt510/bin/qt510-env.sh


mkdir builds
cd builds
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="$TRAVIS_CONFIGURATION_NAME"
cmake --build ./ --clean-first --target all
