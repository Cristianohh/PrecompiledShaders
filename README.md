Intel Corporation - Android Sample Application

Note: “Media Elements” are the images, clip art, animations, sounds, music, shapes, video clips, 2D Images, 2D and 3D Mesh’s and mesh data, animation and animation data, and Textures included in the software. This license does not grant you any rights in the Media Elements and you may not reproduce, prepare derivative works, distribute, publicly display, or publicly perform the Media Elements.

Note: The source code sample is provided under the BSD license.  See the license folder within the sample source directory for additional details.

=== ABOUT ===
Programmatically compiling all shaders the first time an application is run and saving the binaries for reuse can significantly reduce load times for games in subsequent runs. The OpenGL* ES 3.0 sample code introduced here demonstrates a simple implementation of this capability.

=== INSTALLATION ===
Navigate to PrecompiledShaders\projects\android with a device connected, or the emulator running.
Run the following:
	android update project –p .
	ndk-build
	ant debug
	ant installd
