cmake_minimum_required(VERSION 2.8.3)

set( BALL_IMAGE_SRCS
  BallImage.cpp
  )

set( BALL_IMAGE_HEADERS
  BallImage.h
  )

Qt4_wrap_cpp( BALL_IMAGE_HEADERS_MOC ${BALL_IMAGE_HEADERS} )

add_library(
  ballimage
    STATIC
    ${BALL_IMAGE_SRCS}
    ${BALL_IMAGE_HEADERS_MOC}
    )

target_link_libraries(
  ballimage
  messages
  portals
  tool_common
  support
  ${QT_LIBRARIES}
  )
