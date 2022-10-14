import os  
import shutil
import platform
import distro

from zipfile import ZipFile

import tarfile

if not platform.python_version().startswith('3'):
    print("Please use python3 to run this script")
    exit()

def extract(file_path, destinty_path):
    print("Extracting " + file_path)
    if file_path.endswith("zip"):
        with ZipFile(file_path) as zipObj:
            zipObj.extractall(destinty_path)

    if file_path.endswith("tar.xz"):
        with tarfile.open(name=file_path, mode='r:xz') as tarObj:
            tarObj.extractall(destinty_path)

cwd = os.path.dirname(os.path.realpath(__file__))
cwd = cwd + "/.."
os.chdir(cwd)
#print(cwd)

destiny="dependencies"
origin_path_base="scripts/dependencies-zip"

if os.path.isdir(destiny):
    shutil.rmtree(destiny)

os.mkdir(destiny)

system_name = platform.system()
system_info = str(platform.uname())

print(system_name)
print(system_info)

if system_name == "Linux" or system_name == "Linux2":
    # linux
    distro_id = distro.id()
    if distro_id == "ubuntu":
        os.system("sudo apt-get -y update")
        os.system("sudo apt-get -y install build-essential wget zlib1g-dev unzip cmake cmake-data")
    elif distro_id == "manjaro":
        os.system("sudo pacman -Syy")
        os.system("sudo pacman -Sy base-devel wget unzip cmake")

elif system_name == "Darwin":
    # OS X
    pass
elif system_name == "Win32":
    # Windows...
    pass
# elif ANDROID:
#     # Windows...
#     pass
# elif IOS:
#     # Windows...
#     pass

# ------------------------------------------------------------------------

# JSON https://github.com/nlohmann/json
extract(os.path.join(origin_path_base, "json-3.9.1.zip"), destiny)

# ------------------------------------------------------------------------