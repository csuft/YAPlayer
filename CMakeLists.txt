CMAKE_MINIMUM_REQUIRED(VERSION 3.8)
PROJECT(YAPlayer C CXX)

################################################################################
# Compiler and linker options
ADD_DEFINITIONS(-Wall -DWIN32_LEAN_AND_MEAN)

SET(CMAKE_AUTOMOC ON)
SET(CMAKE_AUTORCC ON) 
SET(CMAKE_CXX_STANDARD 11) 

################################################################################
# Sources and libraries
FILE(GLOB CONTROLS "${CMAKE_CURRENT_SOURCE_DIR}/src/controls/*")
FILE(GLOB FORMS "${CMAKE_CURRENT_SOURCE_DIR}/src/forms/*")
FILE(GLOB APPLICATION "${CMAKE_CURRENT_SOURCE_DIR}/src/main/*")
FILE(GLOB UTILITIES "${CMAKE_CURRENT_SOURCE_DIR}/src/utilities/*") 
FILE(GLOB RENDER "${CMAKE_CURRENT_SOURCE_DIR}/src/render/*")  
 
SET(RESOURCES 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/YAPlayer.rc 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/resources/YAPlayer.qrc)  

FIND_PACKAGE(Qt5 COMPONENTS Core Widgets Gui REQUIRED) 
qt5_wrap_ui(UI_HEADERS ${FORMS})

INCLUDE_DIRECTORIES(
${CMAKE_CURRENT_BINARY_DIR}/
${CMAKE_CURRENT_SOURCE_DIR}/external/breakpad/include 
${CMAKE_CURRENT_SOURCE_DIR}/external/ffmpeg/include)

IF(WIN32) 
  LINK_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/external/breakpad/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/external/ffmpeg/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/external/event/lib
  ${CMAKE_CURRENT_SOURCE_DIR}/external/openal/lib)  
  SET(LINK_LIBS 
  		avcodec.lib
		avformat.lib
		avutil.lib
		swscale.lib
		swresample.lib 
		event.lib
		ws2_32.lib 
		OpenAL32.lib 
		google-breakpad-release.lib) 
ENDIF()

################################################################################
# Compile target
ADD_EXECUTABLE ( ${PROJECT_NAME} 
				${CONTROLS} 
				${UI_HEADERS} 
				${APPLICATION}
				${UTILITIES}
				${RESOURCES}
				${RENDER})
# https://www.kdab.com/using-cmake-with-qt-5/
qt5_use_modules(${PROJECT_NAME} Core Widgets Gui)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${LINK_LIBS})