import os
import re

def __get_filepaths(directory):
    file_paths = []
    extensions = ['.cpp', '.h', '.l', '.y', '.yx']

    for root, directories, files in os.walk(directory):
        for filename in files:
            if 'thirdparty' in root:
                continue
            extension = os.path.splitext(filename)[1]
            if extension in extensions:
                filepath = os.path.join(root, filename)
                file_paths.append(filepath)

    return file_paths

def __remove_headers(pattern, text):
    return pattern.sub(r'', text)

def __remove_doxygen(text):
    text = re.sub(r'(.*)\/\/\!(\s)?<(.*)', r'\1//\3', text)
    return text

full_file_paths = __get_filepaths(os.path.abspath('../../'))
remove_headers_pattern = re.compile(r'\/\*\!(?:[^\n]*(\n+))+?(\s*)?(.*)\*\/(\n+)', re.VERBOSE | re.MULTILINE)

for file in full_file_paths:
    print (file)
    ifile = open(file, 'r')
    text = ifile.read()
    ifile.close()
    text = __remove_headers(remove_headers_pattern, text)
    text = __remove_doxygen(text)
    ofile = open(file, 'w')
    ofile.write(text)
