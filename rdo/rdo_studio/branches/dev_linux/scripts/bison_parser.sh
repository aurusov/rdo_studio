###############################################################################
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################
# bison files generator shell script
###############################################################################

cd simulator/compiler/parser/

bison rdodpt.y -g --defines=rdogramma.h -pdpt -ordogramdpt.cpp
bison rdoevn.y -g --defines=rdogramma.h -pevn -ordogramevn.cpp
bison rdoevn_preparse.y -g --defines=rdogramma.h -pevn_preparse_ -ordogramevn_preparse.cpp

bison rdofrm.y -g --defines=rdogramma.h -pfrm -ordogramfrm.cpp
bison rdofun.y -g --defines=rdogramma.h -pfun -ordogramfun.cpp
bison rdopat.y -g --defines=rdogramma.h -ppat -ordogrampat.cpp

bison rdopmd.y -g --defines=rdogramma.h -ppmd -ordogrampmd.cpp
bison rdoproc_opt.y -g --defines=rdogramma.h -pproc_opr_ -ordogramproc_opr.cpp
bison rdoproc_rss.y -g --defines=rdogramma.h -pproc_rss_ -ordogramproc_rss.cpp

bison rdoproc_rtp.y -g --defines=rdogramma.h -pproc_rtp_ -ordogramproc_rtp.cpp
bison rdorss.y -g --defines=rdogramma.h -prss -ordogramrss.cpp
bison rdortp.y -g -v --defines=rdogramma.h -prtp -ordogramrtp.cpp

bison rdosmr_file.y -g --defines=rdogramma.h -psmr_file_ -ordogramsmr_file.cpp
bison rdosmr_sim.y -g --defines=rdogramma.h -psmr_sim_ -ordogramsmr_sim.cpp

cd ../../..