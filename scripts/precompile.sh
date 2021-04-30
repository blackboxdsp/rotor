#!/bin/bash

# Generates "PhantomData.h" (compiles binary resources) and copies to src directory. 

# CAUTION: This script needs to be run if any resources are added to make sure that
# they are compiled into the build!

start_time=$(date +%s)

PLUGIN_NAME=Rotor
BUILD_TYPE=Debug
FILENAME=${PLUGIN_NAME}Data.h

for i in "$@"; do
    case $i in
    -b=*|--build-type=*)
        BUILD_TYPE="${i#*=}"
        shift
        ;;
    esac
done

echo -e "Precompiling ${PLUGIN_NAME} resources...\n"

rm -rf ./bin

cmake -B bin .
echo -e "\n[Success] Configured ${PLUGIN_NAME} for CMake build!\n"

cmake --build bin --config ${BUILD_TYPE} --target "${PLUGIN_NAME}_All"
echo -e "\n[Success] Built ${PLUGIN_NAME}!\n"

rm -f src/${FILENAME}

cp -f bin/juce_binarydata_${PLUGIN_NAME}Data/JuceLibraryCode/${FILENAME} src/${FILENAME}
echo -e "[Success] Copied generated header file to src directory!\n"

convertsecs() {
    ((m = (${1} % 3600) / 60))
    ((s = ${1} % 60))
    printf "%02dm %02ds\n" $m $s
}

end_time=$(date +%s)
execution_time=$(expr $end_time - $start_time)
echo -e "Total time elapsed:    $(convertsecs $execution_time)"\

me=`basename "$0"`
echo -e "Script name:           ${me}"
