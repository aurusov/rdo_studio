###############################################################################
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################
# bison files generator shell script
###############################################################################

cd simulator/compiler/parser/
bison rdortp.y -g -v --defines=rdogramma.h -prtp -ordogramrtp.cpp