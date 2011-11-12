###############################################################################
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################
# bison files generator shell script
###############################################################################

cd simulator/compiler/parser/

bison rdodpt.y -pdpt -ordogramdpt.cpp;
bison rdoevn.y -pevn -ordogramevn.cpp;
bison rdoevn_preparse.y -pevn_preparse_ -ordogramevn_preparse.cpp;

bison rdofrm.y -pfrm -ordogramfrm.cpp;
bison rdofun.y -pfun -ordogramfun.cpp;
bison rdopat.y -ppat -ordogrampat.cpp;

bison rdopmd.y -ppmd -ordogrampmd.cpp;
bison rdoproc_opt.y -pproc_opr_ -ordogramproc_opr.cpp;
bison rdoproc_rss.y -pproc_rss_ -ordogramproc_rss.cpp;

bison rdoproc_opr.y -pproc_opr_ -ordogramproc_opr.cpp;
bison rdorss.y -prss -ordogramrss.cpp;
bison rdortp.y -g -v --defines=rdogramma.h -prtp -ordogramrtp.cpp;

bison rdosmr_file.y -psmr_file_ -ordogramsmr_file.cpp;
bison rdosmr_sim.y -psmr_sim_ -ordogramsmr_sim.cpp;

cd ../../..