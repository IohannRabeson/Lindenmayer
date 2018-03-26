set -e

pwd

mkdir builds
cd builds
cmake .. -G "Unix Makefiles" -DCMAKE_BUILD_TYPE="$TRAVIS_CONFIGURATION_NAME"
cmake --build ./ --clean-first --target all

./tests/unit_tests

cd -
