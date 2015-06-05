// -----------------------------------------------------------------------------
// libDDG -- Viewer.h
// -----------------------------------------------------------------------------
//
// Viewer provides a graphical user interface (GUI) for inspecting and
// interacting with a Mesh object.  Viewer methods are static in order
// to make them compatible with GLUT callbacks.
//

#ifndef DDG_VIEWER_H
#define DDG_VIEWER_H

#include <GLUT/glut.h>
#include "Mesh.h"
#include "Octree.h"
#include "Camera.h"
#include "Shader.h"

class OctreeCell;

namespace DDG
{
    class Viewer {

    public:
        // initialization
        static void init( void );

    protected:
        // init
        static void initGLUT( void );
        static void initGLSL( void );
        
        // GLUT callbacks
        static void display( void );
        static void idle( void );

        static void keyboard(unsigned char c, int x, int y); // keyboard callbacks
        static void special( int i, int x, int y );

        static void mouse(int button, int state, int x, int y); // mouse callbacks
        static void motion( int x, int y );
        static void menu( int value );
        static void view( int value );
        
        // menu functions
        static void mProcess( void );
        static void mResetMesh( void );
        static void mWriteMesh( void );
        static void mExit( void );
        static void mPolygons(void);
        static void mWireframe( void );
        static void mOctree( void );
        static void mBoundingbox(void);
        static void mZoomIn( void );
        static void mZoomOut( void );
        static void mBoundingBox(void);
        static void mScreenshot( void );
        
        // unique identifiers for menus
        enum {
            menuProcess,
            menuResetMesh,
            menuWriteMesh,
            menuExit,
            menuWireframe,
            menuPolygons,
            menuBoundingbox,
            menuOctree,
            menuZoomIn,
            menuZoomOut,
            menuScreenshot
        };
        
        // draw routines
        static void setGL( void );
        static void setLighting( void );
        static void setMeshMaterial( void );
        static void callDisplayList( void );
        static void updateDisplayList( void );
        static void drawScene( void );
        static void drawPolygons( void );
        static void drawWireframe( void );
        static void drawOctree( void );
        static void drawBondingBox(BoundingBox box);
        static void drawVertices( void );
        static void drawSelectedVertices( void );
        static void drawIsolatedVertices( void );
        static void pickVertex(int x, int y);
        
        static void storeViewerState( void );
        static void restoreViewerState( void );


    public:
        // surface mesh visualized by Viewer
        static Mesh mesh;

        // Octree Visualized by Viewer
        static Octree octree;

        static int windowSize[2];

        static bool renderWireframe;
        static bool renderPolygons;
        static bool renderOctree;
        static bool renderBoundingBox;

        // keeps track of view state
        static Camera camera;

        // display list for mesh
        static GLuint surfaceDL;

        // shader used to determine appearance of surface
        static Shader shader;

    };
}

#endif

