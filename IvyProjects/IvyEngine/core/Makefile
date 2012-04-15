
CPPFILES = IvyUtils.cpp     \
           IvyShader.cpp    \
           IvyPerf.cpp      \
           IvyImporter.cpp  \
           IvyCamera.cpp    \
           IvyInput.cpp     \
           IvyTexture.cpp

default: Ivy

CompileIvy:
	g++ -c $(CPPFILES)

Ivy: CompileIvy
	ar rsv *.o



clean:
	rm *.o *.a

