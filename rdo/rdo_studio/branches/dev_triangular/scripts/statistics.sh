###############################################################################
# Copyright (c) 2011 Evgeny Proydakov <lord.tiran@gmail.com>
###############################################################################
# statistics shell script
###############################################################################

# message info
echo -en "\033[31mRun a script to collect statistics on repository  ...\033[0m"
echo ""

# message build system statistics
echo ""
echo -en "\033[36mSearch files of build systems  ...  \033[0m"
find -P . -name "CMakeLists.txt" | grep -vi "build*" | wc -l
echo -en "\033[36mSearch the number of lines in the files of build systems ... \033[0m"
find -P . -name "CMakeLists.txt" | grep -vi "build*" | xargs cat|wc -l

# message scripts statistics
echo ""
echo -en "\033[33mSearch scripts  ...  \033[0m"
find -P . -name "*.sh" | grep -vi "build*" | wc -l
echo -en "\033[33mSearch the number of lines in scripts ... \033[0m"
find -P . -name "*.sh" | grep -vi "build*" | xargs cat|wc -l

# message headers statistics
echo ""
echo -en "\033[35mSearch headers  ...  \033[0m"
find -P . -name "*.h" -o -name "*inl"| grep -vi "build*" | wc -l
echo -en "\033[35mCount the number of lines in the header files ...  \033[0m"
find -P . -name "*.h" -o -name "*inl"| grep -vi "build*" | xargs cat|wc -l
# message sources statistics
echo -en "\033[35mSearch sources  ...  \033[0m"
find -P . -name "*.cpp" | grep -vi "build*" | wc -l
echo -en "\033[35mCount the number of lines in source files  ...  \033[0m"
find -P . -name "*.cpp" | grep -vi "build*" | xargs cat|wc -l
