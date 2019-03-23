import sys

if len(sys.argv) != 4:
    print("usage: <this script> dictionary_name enum_list_file_path scope")
    sys.exit()

dictionary_name = sys.argv[1]
enum_list = sys.argv[2]
scope = sys.argv[3]
lines = []
with open(enum_list, 'r') as enum_list_file:
    lines = enum_list_file.readlines()
print("static std::map<{}, std::string> const {} = \n{{".format(scope, dictionary_name))
counter = 0
for line in lines:
    line = line.strip()
    if len(line) > 0 and line[-1] == ',':
        line = line[:-1]
    if counter > 0:
        print("")
    print("\t{{ {0}::{2}, \"{2}\" }}".format(sys.argv[3], dictionary_name, line), end='')
    if counter < len(lines) - 1:
        print(",", end='')
    counter = counter + 1
print("\n};")
