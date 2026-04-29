import sys
import os
import getopt
import platform
import shutil

cwd = os.path.dirname(os.path.realpath(__file__))
cwd = cwd + "/.."
os.chdir(cwd)
print(cwd) 
  
buildDir="build"
buildType="Debug"
buildUnitTests=False
buildIntegrationTests=False
buildTools=False
enableLogs=False

argv = sys.argv[1:]

try:
  opts, args = getopt.getopt(argv, ":ruixlch")
except:
  print("Error parsing opts")

for opt, arg in opts:
    if opt in ['-c']:
      os.system('./scripts/clean.sh')
    elif opt in ['-r']:
      buildType="Release"
    elif opt in ['-u']:
      buildUnitTests=True
    elif opt in ['-i']:
      buildIntegrationTests=True
    elif opt in ['-x']:
      buildTools=True
    elif opt in ['-l']:
      enableLogs=True

if not os.path.isdir(buildDir):
      os.mkdir(buildDir)

os.chdir(buildDir)

system_name = platform.system()
system_info = str(platform.uname())

print(system_name)
print(system_info)

cmake_generator = ""
if system_name == "Linux" or system_name == "Linux2":
    # linux
    cmake_generator = '-G "Unix Makefiles"'
elif system_name == "Darwin":
    # OS X
    pass
elif system_name == "Windows":
    # Windows...
    cmake_generator = '-G "Visual Studio 17 2022"'
# elif ANDROID:
#     # Windows...
#     pass
# elif IOS:
#     # Windows...
#     pass

# -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++
buildCommand = (
    f"cmake {cmake_generator} "
    f"-DCMAKE_C_COMPILER=/usr/bin/clang "
    f"-DCMAKE_CXX_COMPILER=/usr/bin/clang++ "
    f"-DCMAKE_BUILD_TYPE={buildType} "
    f"-DBUILD_UNIT_TESTS={buildUnitTests} "
    f"-DBUILD_INTEGRATION_TESTS={buildIntegrationTests} "
    f"-DBUILD_TOOLS={buildTools} "
    f"-DENABLE_LOGS={enableLogs} .."
)
os.system(buildCommand)

if system_name == "Linux" or system_name == "Linux2":
    # linux
    os.system('make -j8')
elif system_name == "Darwin":
    # OS X
    pass
elif system_name == "Windows":
    # Windows...
    # Requires msbuild added to the PATH
    os.system('msbuild compiler.sln')
# elif ANDROID:
#     # Windows...
#     pass
# elif IOS:
#     # Windows...
#     pass

os.chdir("..")

##########################################
########## POST BUILD ###########
##########################################

compileCommandsJson = "compile_commands.json"
shutil.copy(os.path.join("build", compileCommandsJson), compileCommandsJson)