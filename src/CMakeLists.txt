
FIND_PACKAGE(Qt4 4.5.0 COMPONENTS QtCore QtGui QtSql REQUIRED)
INCLUDE(CMakeCommons.txt)

include_directories( . ${CMAKE_CURRENT_BINARY_DIR} libfarec)

#################3

SET(farec_ui_SRCS
  ui/farecmainwin
  ui/GradientDisplay
  ui/Graph
)

SET(farec_SRCS
  farec
  FarecApp
)

STRING(TOUPPER ${CMAKE_BUILD_TYPE} MYCBT)
IF( ${MYCBT} STREQUAL DEBUG )
	add_definitions("-DDEBUG_KRZYS")
	
	SET(farec_SRCS ${farec_ui_SRCS}	${farec_SRCS} ui/farecmainwin_tests )
ENDIF()

SET(farec_UIS
	ui/farecmainwin.ui
	ui/GradientDisplay.ui
)

SET(farec_MOCS_H
	ui/farecmainwin.hh
	ui/GradientDisplay.hh
	ui/Graph.hh
)

QT4_WRAP_CPP(farec_MOCS_SRCS ${farec_MOCS_H})


QT4_WRAP_UI(farec_UIS_H ${farec_UIS})

ADD_SUBDIRECTORY(libfarec)

ADD_EXECUTABLE(farec ${farec_SRCS} ${farec_UIS_H} ${farec_MOCS_SRCS})

TARGET_LINK_LIBRARIES(farec ${QT_LIBRARIES} fr)