cmake_minimum_required(VERSION 3.0.0)
project(kTerminal VERSION 0.1.0)

set(SOURCE_PATH ${CMAKE_SOURCE_DIR}/source)
set(COMMAND_PATH ${CMAKE_SOURCE_DIR}/cmd)
set(KSERIAL_PATH ${CMAKE_SOURCE_DIR}/kserial)

add_executable(ks
    main.c

    ${SOURCE_PATH}/serial.c
    ${SOURCE_PATH}/kFile.c
    ${SOURCE_PATH}/kLogger.c
    ${SOURCE_PATH}/kString.c

    ${COMMAND_PATH}/kCommand.c
    ${COMMAND_PATH}/kCommandHelp.c
    ${COMMAND_PATH}/kCommandTarget.c
    ${COMMAND_PATH}/kCommandTwi.c

    ${KSERIAL_PATH}/kSerial.c
)

include_directories(
    ${SOURCE_PATH}
    ${COMMAND_PATH}
    ${KSERIAL_PATH}
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})

set_target_properties(ks PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/out"
)
