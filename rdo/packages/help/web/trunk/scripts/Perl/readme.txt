Данная инструкция предназначена для того, чтобы на основе файлов проекта QT получить HTML-файлы для сайта документации РДО

1. установить интерпретатор perl (проверено на ActivePerl: http://www.activestate.com/activeperl/downloads )
2. перейти в директорию app/rdo_studio_mfc/help/
3. убедиться, что там находятся файлы rdo_studio_rus.qhp и rdo_lang_rus.qhp
4. переместить в эту директорию файлы help-index-head.tmp, help-index-body.tmp, keyword-index-head.tmp, keyword-index-body.tmp
5. переместить в эту директорию файлы help-index.pl и keyword-index.pl - сами скрипты.
6. запустить нужный скрипт


Для редактирования и отладки скрипта можно использовать DWIM (http://padre.perlide.org/download.html)