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

SET(TEST_MODEL_FILES
    ${ARRAY_MODEL}
    ${CREATE_RES_MODEL}
    ${EVENT_QS_MODEL}
    ${FMS_EVENT_MODEL}
    ${FMS_PLANING_MODEL}
    ${POLICLINIC_MODEL})

CONFIGURE_FILE(${CMAKE_CURRENT_SOURCE_DIR}/config.h.cmake ${CMAKE_CURRENT_SOURCE_DIR}/config.h)

SET(CONFIG_FILES
    config.h)

SET(RDO_STUDIO_CONSOLE_APP ${EXECUTABLE_OUTPUT_PATH}/rdo)
SET(RDO_STUDIO_CONSOLE_TEST_PATH ${CMAKE_CURRENT_SOURCE_DIR})

ADD_BOOST_TEST(test_rdo_studio_console ${MAIN_FILE} ${TEST_MODEL_FILES} ${CONFIG_FILES})

ADD_DEPENDENCIES(test_rdo_studio_console rdo)
ADD_DEPENDENCIES(test_rdo_studio_console rdo_utils)

TARGET_LINK_LIBRARIES(test_rdo_studio_console rdo_utils)
