LIBNAME           = drotek_f9p_rover

CCFLAGS           = -Wall -Wextra -Wno-narrowing -std=c++11 -shared -fPIC
INCLUDEFLAGS      = -isystem./include \
					`python3 -m pybind11 --includes`
OPTIMIZATIONFLAGS = -O3 -mtune=native -march=native -DNDEBUG
PARALLELISMFLAGS  = -fopenmp -ltbb 
PYBIND11FLAGS     = -shared -fPIC `python3 -m pybind11 --includes` -o ${LIBNAME}`python3-config --extension-suffix` 

LDFLAGS           = -lblas -llapack -DHAVE_CBLAS=1 -ltbb -lserial


SOURCES_LIB     = src/GPSFix.cpp src/GPSService.cpp src/NMEACommand.cpp src/NMEAParser.cpp src/NumberConversion.cpp

all:
	g++ ${CCFLAGS} ${INCLUDEFLAGS} ${OPTIMIZATIONFLAGS} ${PARALLELISMFLAGS} ${LIBNAME}.cpp HAL_Sirius_F9P_Rover.cc ${SOURCES_LIB} -o ${LIBNAME}`python3-config --extension-suffix` ${LDFLAGS}

clean:
	rm -f ${LIBNAME}`python3-config --extension-suffix`
	rm -f test_cpp
