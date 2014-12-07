import os
import re

def __get_filepaths(directory):
    file_paths = []

    for root, directories, files in os.walk(directory):
        for filename in files:
            extension = os.path.splitext(filename)[1]
            if extension == '.cpp' or extension == '.h':
                filepath = os.path.join(root, filename)
                file_paths.append(filepath)

    return file_paths

full_file_paths = __get_filepaths(os.path.abspath('../../'))
pattern = re.compile(r'\/\*\!(?:[^\n]*(\n+))+?(\s*)?(.*)\*\/(\n+)', re.VERBOSE | re.MULTILINE)
for file in full_file_paths:
    print (file)
    ifile = open(file, 'r')
    text = ifile.read()
    ifile.close()
    text = pattern.sub(r'', text)
    ofile = open(file, 'w')
    ofile.write(text)
