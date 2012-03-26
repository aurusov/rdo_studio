#!/bin/bash

#echo "Enter the filename for log and press Enter or just press Enter:"
#read filehandle
#if [ -s $filehandle ]; then
filehandle="diff.log"
echo "Name \"diff.log\" will be used instead"
#fi

echo "Current dir" > $filehandle
svn diff >> $filehandle
echo "" >> $filehandle

ls -R | grep "\./" | sed 's/.$//g' | while read dir; do
svn diff "$dir" >> $filehandle
done
