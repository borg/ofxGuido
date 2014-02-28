--------------------------------------------------------------------------
                       GUIDO Engine Library Qt binding
--------------------------------------------------------------------------

Preliminary note: this package doesn't contain the GUIDO Engine Library 
itself. In order to use the GUIDO-Qt binding classes, you must first
download the GUIDOEngine "dev kit" packages for your platform.

GUIDO-Qt binding classes have been developped to use the
Guidolib in Qt applications. This binding basically allows you
to parse Guido Music Notation files or strings and draw the resulting 
GUIDO Score with a QPainter ; "ready-to-use" QGuidoWidget and
QGuidoGraphicsItem are already implemented.

As Qt is a cross-platform toolkit, you can use the Guido Qt binding 
classes on any platform. You need to install the platfrom native Guidolib
before using the Guido-Qt binding. Next you just have to link your Qt 
application against the  corresponding platfrom native Guidolib.

--------------------------------------------------------------------------
			Package contents
--------------------------------------------------------------------------

- doc: 
	- documentation on the GUIDO Music Notation ;
	- documentation on the GUIDOEngine API ;
	- documentation on the GUIDO-Qt binding classes.
- src:
	- GuidoQt: the static library containing the GUIDO-Qt binding 
	classes ;
	- include: headers from the GUIDOEngine ;
	- other folders contains Qt applications using the GUIDO-Qt 
	binding classes. NOTE: YOU NEED THE NATIVE GUIDOENGINE LIBRARY
	TO BUILD THOSE APPLICATIONS. By default, the application will
	look for the GUIDOEngine library in the guidolib-X-devkit-Qt/src
	folder, so you should manually put it there.
	- GraphicsSceneMainWindow is a static library used by the 
	GuidoSceneComposer application. It doesn't use the GUIDOEngine.
	(it is notably used by another GRAME application, 
	FaustGIDE "A Graphic Faust IDE")
- guido2.ttf is the font used by the GUIDO Engine. You'll need to install 
	this font on your system.

--------------------------------------------------------------------------
The source code of the GUIDO Qt binding classes are available from 
sourceforge under the Mozilla Public license.

For more informations : http://sourceforge.net/projects/guidolib


--------------------------------------------------------------------------
Acknowledgments
--------------------------------------------------------------------------
The Guido project originates from Ph.D.s conducted by Holger Hoos, Kai Renz and 
J�rgen Kilian at the TU Darmstadt.
The Guido engine has been designed by Kai Renz. It became an open source library in 
December 2002 at the initiative of the Grame Computer Music Research Lab. Since then,
many persons have been contributing to the Guido library project.
I would like to thank them and especially: 
J. Scott Amort, J�r�me Berthet, Samuel Brochot, Yannick Chapuis, Christophe Daudin,
Colas Decron, Torben Hohn, Camille Le Roi, St�phane Letz, Fran�ois Levy, 
Benjamen Ruprechter, Mike Solomon

Dominique Fober
