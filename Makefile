# cs335 project
# to compile your project, type make and press enter
#
# To disable OpenAL sound, place comments on the #define
# and library statements. 
# Like this...
#
#-D USE_OPENAL_SOUND \
#/usr/lib/x86_64-linux-gnu/libopenal.so \
#/usr/lib/x86_64-linux-gnu/libalut.so


all: project

project: projectMain.cpp ppm.cpp timers.cpp codyG.cpp kyleS.cpp derrickA.cpp codyW.cpp
	g++ projectMain.cpp ppm.cpp timers.cpp codyG.cpp codyW.cpp kyleS.cpp derrickA.cpp libggfonts.a -Wall -oproject -lX11 -lGL -lGLU -lm \
  -D USE_OPENAL_SOUND \
  	/usr/lib/x86_64-linux-gnu/libopenal.so \
  	/usr/lib/x86_64-linux-gnu/libalut.so

clean:
	rm -f project
	rm -f *.o

