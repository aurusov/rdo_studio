#==============================================================================
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
#==============================================================================

SET(SOURCE_FILES
    main.cpp
    )

ADD_BOOST_TEST(test_rdo_utils ${SOURCE_FILES} ${RESOURCE_FILES})
ADD_DEPENDENCIES(test_rdo_utils rdo_utils)
TARGET_LINK_LIBRARIES(test_rdo_utils rdo_utils)
