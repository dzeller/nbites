cmake_minimum_required(VERSION 2.8.3)

set( BALL_TEST_SRCS
  BallTest.cpp
  )

set( BALL_TEST_HEADERS
  BallTest.h
  )

#Qt4_wrap_cpp( BALL_IMAGE_HEADERS_MOC ${BALL_IMAGE_HEADERS} )

add_library( balltest
    STATIC
    ${BALL_TEST_SRCS}
    )

target_link_libraries(
  balltest
  ballimage
  messages
  portals
  support
  unlog
  )
