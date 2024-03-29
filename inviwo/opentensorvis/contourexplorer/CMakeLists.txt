ivw_module(ContourExplorer)

set(HEADER_FILES
    include/inviwo/contourexplorer/util/segmentationcolorhelper.h
    include/inviwo/contourexplorer/contourexplorermodule.h
    include/inviwo/contourexplorer/contourexplorermoduledefine.h
    include/inviwo/contourexplorer/processors/contourexplorerprocessor.h
    include/inviwo/contourexplorer/processors/contourtreetosegmentationvolume.h
    include/inviwo/contourexplorer/processors/criticalpointstomeshprocessor.h
    include/inviwo/contourexplorer/processors/segmentationlegendprocessor.h
    include/inviwo/contourexplorer/processors/segmentationvolumetransferfunctionprocessor.h
    include/inviwo/contourexplorer/processors/volumelabelselectionprocessor.h
)
ivw_group("Header Files" ${HEADER_FILES})

set(SOURCE_FILES
    src/util/segmentationcolorhelper.cpp
    src/contourexplorermodule.cpp
    src/processors/contourexplorerprocessor.cpp
    src/processors/contourtreetosegmentationvolume.cpp
    src/processors/criticalpointstomeshprocessor.cpp
    src/processors/segmentationlegendprocessor.cpp
    src/processors/segmentationvolumetransferfunctionprocessor.cpp
    src/processors/volumelabelselectionprocessor.cpp
)
ivw_group("Source Files" ${SOURCE_FILES})

set(SHADER_FILES
	# Add shaders
)
ivw_group("Shader Files" ${SHADER_FILES})

set(TEST_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unittests/contourexplorer-unittest-main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/tests/unittests/colorhelper.cpp
)
ivw_add_unittest(${TEST_FILES})

ivw_create_module(${SOURCE_FILES} ${HEADER_FILES} ${SHADER_FILES})

# Add shader directory to install package
#ivw_add_to_module_pack(${CMAKE_CURRENT_SOURCE_DIR}/glsl)
