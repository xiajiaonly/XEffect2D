libdir = ../XEffeEngine/
linklib = -L/usr/lib/fglrx -lSDL -lSDL_image -lGL -lGLU -lSDL_mixer -lGLEW -L/media/DISK/Linux/lib -lavcodec -lavformat -lavutil
objects = SDL_Test.o XBasicClass.o XBasicFun.o XBasicOpenGL.o XBasicSDL.o XBezierInterpolation.o XBezierSpline.o XEffeEngine.o XEngineLogo.o XFont.o XFontUnicode.o XFrame.o XHTTPRequest.o XLogBook.o XMusic.o XNodeLine.o XNumber.o XResourcePack.o XSocket.o XSound.o XSprite.o XButton.o XCheck.o XCombo.o XControlBasic.o XEdit.o XMouseRightButtonMenu.o XMutiEdit.o XMutiText.o XMutiList.o XProgress.o XRadios.o XSlider.o XCloudParticles.o XEchoParticles.o XFireworkParticles.o XJetParticles.o XParticle.o XSnowParticles.o XPhysicsBasic.o XSpring.o tinystr.o tinyxml.o tinyxmlerror.o tinyxmlparser.o XMovie.o
# XCircleGroup.o
libIncludeDir = -I../XEffeEngine -I/usr/include/GL -I/usr/include/SDL -I../XEffeEngine/XControl -I../XEffeEngine/XParticle -I../XEffeEngine/XPhysics -I../tinyXML -I/media/DISK/linux/include/ffmpeg
SDL_Test:$(objects)
	g++ -o SDL_Test $(objects) $(linklib)
SDL_Test.o:SDL_Test.cpp
	g++ -c SDL_Test.cpp $(libIncludeDir)
#XBasic
XBasicClass.o:$(libdir)XBasicClass.cpp
	g++ -c $(libdir)XBasicClass.cpp $(libIncludeDir)
XBasicFun.o:$(libdir)XBasicFun.cpp
	g++ -c $(libdir)XBasicFun.cpp $(libIncludeDir)
XBasicOpenGL.o:$(libdir)XBasicOpenGL.cpp
	g++ -c $(libdir)XBasicOpenGL.cpp $(libIncludeDir)
XBasicSDL.o:$(libdir)XBasicSDL.cpp
	g++ -c $(libdir)XBasicSDL.cpp $(libIncludeDir)
XBezierInterpolation.o:$(libdir)XBezierInterpolation.cpp
	g++ -c $(libdir)XBezierInterpolation.cpp $(libIncludeDir)
XBezierSpline.o:$(libdir)XBezierSpline.cpp
	g++ -c $(libdir)XBezierSpline.cpp $(libIncludeDir)
XEffeEngine.o:$(libdir)XEffeEngine.cpp
	g++ -c $(libdir)XEffeEngine.cpp $(libIncludeDir)
XEngineLogo.o:$(libdir)XEngineLogo.cpp
	g++ -c $(libdir)XEngineLogo.cpp $(libIncludeDir)
XFont.o:$(libdir)XFont.cpp
	g++ -c $(libdir)XFont.cpp $(libIncludeDir)
XFontUnicode.o:$(libdir)XFontUnicode.cpp
	g++ -c $(libdir)XFontUnicode.cpp $(libIncludeDir)
XFrame.o:$(libdir)XFrame.cpp
	g++ -c $(libdir)XFrame.cpp $(libIncludeDir)
XHTTPRequest.o:$(libdir)XHTTPRequest.cpp
	g++ -c $(libdir)XHTTPRequest.cpp $(libIncludeDir)
XLogBook.o:$(libdir)XLogBook.cpp
	g++ -c $(libdir)XLogBook.cpp $(libIncludeDir)
XMusic.o:$(libdir)XMusic.cpp
	g++ -c $(libdir)XMusic.cpp $(libIncludeDir)
XNodeLine.o:$(libdir)XNodeLine.cpp
	g++ -c $(libdir)XNodeLine.cpp $(libIncludeDir)
XNumber.o:$(libdir)XNumber.cpp
	g++ -c $(libdir)XNumber.cpp $(libIncludeDir)
XResourcePack.o:$(libdir)XResourcePack.cpp
	g++ -c $(libdir)XResourcePack.cpp $(libIncludeDir)
XSocket.o:$(libdir)XSocket.cpp
	g++ -c $(libdir)XSocket.cpp $(libIncludeDir)
XSound.o:$(libdir)XSound.cpp
	g++ -c $(libdir)XSound.cpp $(libIncludeDir)
XSprite.o:$(libdir)XSprite.cpp
	g++ -c $(libdir)XSprite.cpp $(libIncludeDir)
XMovie.o:$(libdir)XMovie.cpp
	g++ -c $(libdir)XMovie.cpp $(libIncludeDir)
#XCircleGroup.o:$(libdir)XCircleGroup.cpp
#	g++ -c $(libdir)XCircleGroup.cpp $(libIncludeDir)
#XControl
XButton.o:$(libdir)XControl/XButton.cpp
	g++ -c $(libdir)XControl/XButton.cpp $(libIncludeDir)
XCheck.o:$(libdir)XControl/XCheck.cpp
	g++ -c $(libdir)XControl/XCheck.cpp $(libIncludeDir)
XCombo.o:$(libdir)XControl/XCombo.cpp
	g++ -c $(libdir)XControl/XCombo.cpp $(libIncludeDir)
XControlBasic.o:$(libdir)XControl/XControlBasic.cpp
	g++ -c $(libdir)XControl/XControlBasic.cpp $(libIncludeDir)
XEdit.o:$(libdir)XControl/XEdit.cpp
	g++ -c $(libdir)XControl/XEdit.cpp $(libIncludeDir)
XMouseRightButtonMenu.o:$(libdir)XControl/XMouseRightButtonMenu.cpp
	g++ -c $(libdir)XControl/XMouseRightButtonMenu.cpp $(libIncludeDir)
XMutiEdit.o:$(libdir)XControl/XMutiEdit.cpp
	g++ -c $(libdir)XControl/XMutiEdit.cpp $(libIncludeDir)
XMutiText.o:$(libdir)XControl/XMutiText.cpp
	g++ -c $(libdir)XControl/XMutiText.cpp $(libIncludeDir)
XMutiList.o:$(libdir)XControl/XMutiList.cpp
	g++ -c $(libdir)XControl/XMutiList.cpp $(libIncludeDir)
XProgress.o:$(libdir)XControl/XProgress.cpp
	g++ -c $(libdir)XControl/XProgress.cpp $(libIncludeDir)
XRadios.o:$(libdir)XControl/XRadios.cpp
	g++ -c $(libdir)XControl/XRadios.cpp $(libIncludeDir)
XSlider.o:$(libdir)XControl/XSlider.cpp
	g++ -c $(libdir)XControl/XSlider.cpp $(libIncludeDir)
#XParticle
XCloudParticles.o:$(libdir)XParticle/XCloudParticles.cpp
	g++ -c $(libdir)XParticle/XCloudParticles.cpp $(libIncludeDir)
XEchoParticles.o:$(libdir)XParticle/XEchoParticles.cpp
	g++ -c $(libdir)XParticle/XEchoParticles.cpp $(libIncludeDir)
XFireworkParticles.o:$(libdir)XParticle/XFireworkParticles.cpp
	g++ -c $(libdir)XParticle/XFireworkParticles.cpp $(libIncludeDir)
XJetParticles.o:$(libdir)XParticle/XJetParticles.cpp
	g++ -c $(libdir)XParticle/XJetParticles.cpp $(libIncludeDir)
XParticle.o:$(libdir)XParticle/XParticle.cpp
	g++ -c $(libdir)XParticle/XParticle.cpp $(libIncludeDir)
XSnowParticles.o:$(libdir)XParticle/XSnowParticles.cpp
	g++ -c $(libdir)XParticle/XSnowParticles.cpp $(libIncludeDir)
#XPhysics
XPhysicsBasic.o:$(libdir)XPhysics/XPhysicsBasic.cpp
	g++ -c $(libdir)XPhysics/XPhysicsBasic.cpp $(libIncludeDir)
XSpring.o:$(libdir)XPhysics/XSpring.cpp
	g++ -c $(libdir)XPhysics/XSpring.cpp $(libIncludeDir)
#TinyXML
tinystr.o:../tinyXML/tinystr.cpp
	g++ -c ../tinyXML/tinystr.cpp $(libIncludeDir)
tinyxml.o:../tinyXML/tinyxml.cpp
	g++ -c ../tinyXML/tinyxml.cpp $(libIncludeDir)
tinyxmlerror.o:../tinyXML/tinyxmlerror.cpp
	g++ -c ../tinyXML/tinyxmlerror.cpp $(libIncludeDir)
tinyxmlparser.o:../tinyXML/tinyxmlparser.cpp
	g++ -c ../tinyXML/tinyxmlparser.cpp $(libIncludeDir)

clean:
	rm -f *.o
