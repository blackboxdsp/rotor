# Checks that the installed version of CMake can understand the following commands
cmake_minimum_required(VERSION 3.15)

# Declaring targets and describing source/ binary directories
project(Rotor VERSION 0.0.1)

# Declare dependency on JUCE (as installed on the local system)
add_subdirectory(JUCE)

# Add the main directory for project
add_subdirectory(Rotor)
