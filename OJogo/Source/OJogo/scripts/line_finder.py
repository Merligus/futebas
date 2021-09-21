from os import listdir
from os.path import isfile, join

code = ["inputEnabled"]

path = "../"
onlyfiles = [f for f in listdir(path) if isfile(join(path, f))]
for filename in onlyfiles:
    with open(f"../{filename}", "r") as f:
        for n, line in enumerate(f):
            for code_i in code:
                if code_i in line:
                    print(f"{line.strip()} in {filename} line #{n+1}")
                    break
