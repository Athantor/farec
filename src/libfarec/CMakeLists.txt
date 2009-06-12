
FIND_PACKAGE(Qt4 4.5.0 COMPONENTS QtCore QtGui REQUIRED)

INCLUDE(../CMakeCommons.txt)

SET(libfarec_exc_SRC
	except/FarecException
	except/FEInvalidParameter
	except/FENoData
)

SET(libfarec_SRC
	CommonOps
	ImgOp
	ImgPrep
	ImgData
	
	FeatExtract
)


SET(libfarec_MOCS_H

)

QT4_WRAP_CPP(libfarec_MOCS_SRCS ${libfarec_MOCS_H})


add_library(fr STATIC ${libfarec_SRC} ${libfarec_exc_SRC} ${libfarec_MOCS_SRCS} )
TARGET_LINK_LIBRARIES(fr ${QT_LIBRARIES} )
