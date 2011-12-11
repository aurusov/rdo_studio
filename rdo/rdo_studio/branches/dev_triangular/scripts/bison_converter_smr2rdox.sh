###############################################################################
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################
# bison files generator shell script
###############################################################################

cd converter/smr2rdox/grammar

bison rdosmr_sim.y -pcnv_smr_sim_ -ordogramsmr_sim.cpp
bison rdosmr_file.y -pcnv_smr_file_ -ordogramsmr_file.cpp
bison rdortp.y -v --defines=rdogramma.h -pcnv_rtp -ordogramrtp.cpp
bison rdorss.y -pcnv_rss -ordogramrss.cpp
bison rdoproc_rtp.y -pcnv_proc_rtp_ -ordogramproc_rtp.cpp
bison rdoproc_rss.y -pcnv_proc_rss_ -ordogramproc_rss.cpp
bison rdoproc_opr.y -pcnv_proc_opr_ -ordogramproc_opr.cpp
bison rdopmd.y -pcnv_pmd -ordogrampmd.cpp
bison rdopat.y -pcnv_pat -ordogrampat.cpp
bison rdoopr.y -pcnv_opr -ordogramopr.cpp
bison rdofun.y -pcnv_fun -ordogramfun.cpp
bison rdofrm.y -pcnv_frm -ordogramfrm.cpp
bison rdodpt.y -pcnv_dpt -ordogramdpt.cpp

cd ../../..