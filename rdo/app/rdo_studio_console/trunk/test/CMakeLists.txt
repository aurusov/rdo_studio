#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================
# Cmake for RDO  STUDIO  CONSOLE  TEST
#==============================================================================

SET(MAIN_FILE 
    main.cpp)

SET(ARRAY_MODEL
    array/array_etalon.pmv
    array/array_etalon.trc
    array/array.dpt
    array/array.frm
    array/array.fun
    array/array.opr
    array/array.pat
    array/array.rdox
    array/array.rss
    array/array.rtp)

SET(CREATE_RES_MODEL
    create_res/create_res_etalon.pmv
    create_res/create_res_etalon.trc
    create_res/create_res.prc
    create_res/create_res.rdox
    create_res/create_res.rss
    create_res/create_res.rtp)

SET(EVENT_QS_MODEL
    event_qs/event_qs_etalon.pmv
    event_qs/event_qs_etalon.trc
    event_qs/event_qs.fun
    event_qs/event_qs.pmd
    event_qs/event_qs.rdox
    event_qs/event_qs.rss
    event_qs/event_qs.rtp)

SET(FMS_EVENT_MODEL
    fms_event/fms_event_etalon.pmv
    fms_event/fms_event_etalon.trc
    fms_event/fms_event.evn
    fms_event/fms_event.fun
    fms_event/fms_event.pmd
    fms_event/fms_event.rdox
    fms_event/fms_event.rss
    fms_event/fms_event.rtp
    fms_event/fms_event.smr)

SET(FMS_PLANING_MODEL
    fms_planing/fms_planing.dpt
    fms_planing/fms_planing_etalon.pmv
    fms_planing/fms_planing_etalon.trc
    fms_planing/fms_planing.evn
    fms_planing/fms_planing.frm
    fms_planing/fms_planing.fun
    fms_planing/fms_planing.pat
    fms_planing/fms_planing.pmd
    fms_planing/fms_planing.prc
    fms_planing/fms_planing.rdox
    fms_planing/fms_planing.rss
    fms_planing/fms_planing.rtp
    fms_planing/fms_planing.smr)

SET(POLICLINIC_MODEL
    policlinic/policlinic.dpt
    policlinic/policlinic_etalon.pmv
    policlinic/policlinic_etalon.trc
    policlinic/policlinic.evn
    policlinic/policlinic.fun
    policlinic/policlinic.pat
    policlinic/policlinic.pmd
    policlinic/policlinic.rdox
    policlinic/policlinic.rss
    policlinic/policlinic.rtp)

SET(SIMPLE_QS_MODEL
    simple_qs/simple_qs.dpt
    simple_qs/simple_qs_etalon.pmv
    simple_qs/simple_qs_etalon.trc
    simple_qs/simple_qs.evn
    simple_qs/simple_qs.fun
    simple_qs/simple_qs.pat
    simple_qs/simple_qs.pmd
    simple_qs/simple_qs.rdox
    simple_qs/simple_qs.rss
    simple_qs/simple_qs.rtp
    simple_qs/simple_qs.smr)

SET(MULTICHANNEL_QS_MODEL
    multichannel_qs/multichannel_qs.dpt
    multichannel_qs/multichannel_qs_etalon.pmv
    multichannel_qs/multichannel_qs_etalon.trc
    multichannel_qs/multichannel_qs.evn
    multichannel_qs/multichannel_qs.fun
    multichannel_qs/multichannel_qs.pat
    multichannel_qs/multichannel_qs.pmd
    multichannel_qs/multichannel_qs.rdox
    multichannel_qs/multichannel_qs.rss
    multichannel_qs/multichannel_qs.rtp
    multichannel_qs/multichannel_qs.smr)

SET(HEIDEL_MODEL
    heidel/Cut1.bmp
    heidel/Cut2.bmp
    heidel/Cut3.bmp
    heidel/Cut4.bmp
    heidel/Cut6.bmp
    heidel/Cut8.bmp
    heidel/Cut9.bmp
    heidel/Cut_m.bmp
    heidel/Fl.bmp
    heidel/Fr1111.bmp
    heidel/Fr3.bmp
    heidel/Fr4.bmp
    heidel/Fr5.bmp
    heidel/heidel.dpt
    heidel/heidel_etalon.pmv
    heidel/heidel_etalon.trc
    heidel/heidel.evn
    heidel/heidel.frm
    heidel/heidel.fun
    heidel/heidel.pat
    heidel/heidel.pmd
    heidel/heidel.rdox
    heidel/heidel.rss
    heidel/heidel.rtp
    heidel/heidel.smr)

SET(TEST_MODEL_FILES
    ${ARRAY_MODEL}
    ${CREATE_RES_MODEL}
    ${EVENT_QS_MODEL}
    ${FMS_EVENT_MODEL}
    ${FMS_PLANING_MODEL}
    ${POLICLINIC_MODEL}
    ${SIMPLE_QS_MODEL}
    ${MULTICHANNEL_QS_MODEL}
    ${HEIDEL_MODEL})

IF(MSVC)
    SET(RDO_STUDIO_CONSOLE_APP "-DRDO_STUDIO_CONSOLE_APP ${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/rdo.exe")
    SET(RDO_STUDIO_CONSOLE_TEST_PATH "-DRDO_STUDIO_CONSOLE_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/")
#    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.msvc.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config.h)
ELSE()
    SET(RDO_STUDIO_CONSOLE_APP "-DRDO_STUDIO_CONSOLE_APP ${EXECUTABLE_OUTPUT_PATH}/rdo")
    SET(RDO_STUDIO_CONSOLE_TEST_PATH "-DRDO_STUDIO_CONSOLE_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR}/")
#    CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.gcc.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config.h)
ENDIF(MSVC)


SET(CONFIG_FILES)
#    config.h)

SET(RDO_STUDIO_CONSOLE_APP ${EXECUTABLE_OUTPUT_PATH}/rdo)
SET(RDO_STUDIO_CONSOLE_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR})

ADD_BOOST_TEST(test_rdo_studio_console ${MAIN_FILE} ${TEST_MODEL_FILES} ${CONFIG_FILES})

ADD_DEPENDENCIES(test_rdo_studio_console rdo)
ADD_DEPENDENCIES(test_rdo_studio_console rdo_utils)

TARGET_LINK_LIBRARIES(test_rdo_studio_console rdo_utils)

#SET_TARGET_PROPERTIES(test_rdo_studio_console PROPERTIES COMPILE_FLAGS ${RDO_STUDIO_CONSOLE_APP})
#SET_TARGET_PROPERTIES(test_rdo_studio_console PROPERTIES COMPILE_FLAGS ${RDO_STUDIO_CONSOLE_TEST_PATH})

IF(MSVC)
    ADD_DEFINITIONS(-DRDO_STUDIO_CONSOLE_APP="${EXECUTABLE_OUTPUT_PATH}/${CMAKE_BUILD_TYPE}/rdo.exe")
ELSE()
    ADD_DEFINITIONS(-DRDO_STUDIO_CONSOLE_APP="${EXECUTABLE_OUTPUT_PATH}/rdo")
ENDIF(MSVC)

ADD_DEFINITIONS(-DRDO_STUDIO_CONSOLE_TEST_PATH="${CMAKE_CURRENT_SOURCE_DIR}/")

IF(MSVC) # lotions for windows #

SOURCE_GROUP(".array_model" FILES
    ${ARRAY_MODEL})

SOURCE_GROUP(".create_res_model" FILES
    ${CREATE_RES_MODEL})

SOURCE_GROUP(".event_qs_model" FILES
    ${EVENT_QS_MODEL})

SOURCE_GROUP(".fms_event_model" FILES
    ${FMS_EVENT_MODEL})

SOURCE_GROUP(".fms_planing_model" FILES
    ${FMS_PLANING_MODEL})

SOURCE_GROUP(".policlinic_model" FILES
    ${POLICLINIC_MODEL})

SOURCE_GROUP(".simple_qs_model" FILES
    ${SIMPLE_QS_MODEL})

SOURCE_GROUP(".multichannel_qs" FILES
    ${MULTICHANNEL_QS_MODEL})

SOURCE_GROUP(".heidel" FILES
    ${HEIDEL_MODEL})

ENDIF(MSVC)
