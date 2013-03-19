#!/bin/sh

file_name="../../app/rdo_studio/res/build_version.h"

echo "#ifndef _RDO_STUDIO_RES_BUILD_VERSION_H_" > $file_name
echo "#define _RDO_STUDIO_RES_BUILD_VERSION_H_" >> $file_name
echo "" >> $file_name
echo -n "static const QString g_buildVersion(\"svn-" >> $file_name
svnversion -n ./../../ >> $file_name
echo "\");" >> $file_name
echo "" >> $file_name
echo "#endif // _RDO_STUDIO_RES_BUILD_VERSION_H_" >> $file_name
