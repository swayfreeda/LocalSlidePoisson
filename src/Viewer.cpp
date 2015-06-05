#include <cmath>
#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>

using namespace std;

#include "Viewer.h"
#include "Image.h"
#include "OctreeCell.h"
#include "Application.h"

namespace DDG
{
    // declare [static] member variables
    Mesh Viewer::mesh;
    GLuint Viewer::surfaceDL = 0;
    int Viewer::windowSize[2] = { 512, 512 };
    Camera Viewer::camera;
    Shader Viewer::shader;
    bool Viewer::renderWireframe = false;
    bool Viewer::renderOctree = false;
    bool Viewer::renderBoundingBox = false;
    bool Viewer::renderPolygons = true;
    Octree Viewer::octree;

    //==================================================init==========================================================//
    void Viewer::init(void) {
        restoreViewerState();
        initGLUT();
        setGL();
        initGLSL();
        updateDisplayList();
        glutMainLoop();
    }

    //==================================================initGLUT======================================================//
    void Viewer::initGLUT(void) {
        int argc = 0;
        vector< vector<char> > argv(1);
        
        // initialize window
        glutInitWindowSize(windowSize[0], windowSize[1]); // size of window
        glutInitDisplayMode( GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH );
        glutInit( &argc, (char**)&argv );
        glutCreateWindow("DDG"); // create window

        // specify callbacks functions
        glutDisplayFunc  ( Viewer::display  );
        glutIdleFunc     ( Viewer::idle     );
        glutKeyboardFunc ( Viewer::keyboard );
        glutSpecialFunc  ( Viewer::special  );
        glutMouseFunc    ( Viewer::mouse    );
        glutMotionFunc   ( Viewer::motion   );
        
        // initialize menus
        int viewMenu = glutCreateMenu( Viewer::view );
        glutSetMenu( viewMenu );
        glutAddMenuEntry( "[f] Wireframe",  menuWireframe    );
        glutAddMenuEntry( "[o] Octree",  menuOctree    );
        glutAddMenuEntry( "[p] Polygons", menuPolygons);
        glutAddMenuEntry( "[↑] Zoom In",    menuZoomIn       );
        glutAddMenuEntry( "[↓] Zoom Out",   menuZoomOut      );
        glutAddMenuEntry( "[b] Bounding Box", menuBoundingbox);
        int mainMenu = glutCreateMenu( Viewer::menu );
        glutSetMenu( mainMenu );
        glutAddMenuEntry( "[space] Process Mesh", menuProcess    );
        glutAddMenuEntry( "[r] Reset Mesh",       menuResetMesh  );
        glutAddMenuEntry( "[w] Write Mesh",       menuWriteMesh  );
        glutAddMenuEntry( "[\\] Screenshot",      menuScreenshot );
        glutAddMenuEntry( "[esc] Exit",           menuExit       );
        glutAddSubMenu( "View", viewMenu );
        glutAttachMenu( GLUT_RIGHT_BUTTON );
    }

    //====================================================initGLSL====================================================//
    void Viewer::initGLSL(void) {
        shader.loadVertex( "shaders/vertex.glsl" );
        shader.loadFragment( "shaders/fragment.glsl" );
    }

    //====================================================menu=========================================================//
    void Viewer::menu(int value) {
        switch( value )
        {
            case( menuProcess ):
                mProcess();
                break;
            case( menuResetMesh ):
                mResetMesh();
                break;
            case( menuWriteMesh ):
                mWriteMesh();
                break;
            case( menuScreenshot ):
                mScreenshot();
                break;
            case( menuExit ):
                mExit();
                break;
            default:
                break;
        }
    }

    //===================================================view========================================================//
    void Viewer::view(int value) {

        switch (value) {
            case(menuPolygons):
                mPolygons();
                break;
            case( menuWireframe ):
                mWireframe();
                break;
            case( menuOctree):
                mOctree();
                break;
            case( menuZoomIn ):
                mZoomIn();
                break;
            case( menuZoomOut ):
                mZoomOut();
                break;
            default:
                break;
        }
    }

    //==================================================keyboard======================================================//
    void Viewer::keyboard(unsigned char c, int x, int y) {

        switch (c) {
            case 'b':
                mBoundingBox();
                break;
            case 'p':
                mPolygons();
                break;
            case 'o':
                mOctree();
                break;
            case 'f':
                mWireframe();
                break;
            case 'w':
                mWriteMesh();
                break;
            case 'r':
                mResetMesh();
                break;
            case '\\':
                mScreenshot();
                break;
            case ' ':
                mProcess();
                break;
            case 27:
                mExit();
                break;
            default:
                break;
        }
    }

    //==================================================special=====================================================//
    void Viewer::special(int i, int x, int y) {
        switch( i )
        {
            case GLUT_KEY_UP:
                camera.zoomIn();
                break;
            case GLUT_KEY_DOWN:
                camera.zoomOut();
                break;
            case 27:
                mExit();
                break;
            default:
                break;
        }
    }

    //====================================================mouse=====================================================//
    void Viewer::mouse(int button, int state, int x, int y) {
        if( ( glutGetModifiers() and GLUT_ACTIVE_SHIFT) and state == GLUT_UP )
            pickVertex(x, y);
        else
            camera.mouse( button, state, x, y );
    }

    //===================================================motion=====================================================//
    void Viewer::motion(int x, int y) {
        camera.motion( x, y );
    }


    //===================================================idle=======================================================//
    void Viewer::idle(void) {
        camera.idle();
        glutPostRedisplay();
    }

    //====================================================storeViewerState==========================================//
    void Viewer::storeViewerState(void) {

        ofstream out( ".viewer_state.txt" );
        
        out << camera.rLast[0] << endl;
        out << camera.rLast[1] << endl;
        out << camera.rLast[2] << endl;
        out << camera.rLast[3] << endl;
        
        GLint view[4];
        glGetIntegerv( GL_VIEWPORT, view );
        out << view[2] << endl;
        out << view[3] << endl;
    }

    //====================================================restoreViewerState========================================//
    void Viewer :: restoreViewerState( void )
    {
        ifstream in( ".viewer_state.txt" );
        if( !in.is_open() ) return;
        
        in >> camera.rLast[0];
        in >> camera.rLast[1];
        in >> camera.rLast[2];
        in >> camera.rLast[3];
        in >> windowSize[0];
        in >> windowSize[1];
    }

    //===================================================mProcess====================================================//
    // process mesh
    void Viewer::mProcess(void) {
        Application app;
        app.run(mesh, octree);
        updateDisplayList();
    }

    //===================================================resetMesh===================================================//
    void Viewer::mResetMesh(void) {
        mesh.reload();
        updateDisplayList();
    }

    //===================================================wWriteMesh==================================================//
    void Viewer::mWriteMesh(void) {
        mesh.write( "out.obj" );
    }

    //===================================================mExit=======================================================//
    void Viewer::mExit(void) {
        //storeViewerState();
        exit( 0 );
    }

    //===================================================mWireframe==================================================//
    void Viewer::mWireframe(void) {
        renderWireframe = !renderWireframe;
        updateDisplayList();
    }

    //===================================================mOctree======================================================//
    // display mOctree
    void Viewer::mOctree(void) {
        renderOctree = !renderOctree;
        updateDisplayList();
    }

    //===================================================mBoundingBox=================================================//
    void Viewer::mBoundingBox() {
        renderBoundingBox = !renderBoundingBox;
        updateDisplayList();
    }

    //===================================================mZoomIn======================================================//
    void Viewer::mZoomIn(void) {
        camera.zoomIn();
    }

    //===================================================mZoomOut====================================================//
    void Viewer::mZoomOut(void) {
        camera.zoomOut();
    }

    //===================================================mScreenshot=================================================//
    void Viewer::mScreenshot(void) {
        static int index = 0;
        
        // get window width and height
        GLint view[4];
        glGetIntegerv( GL_VIEWPORT, view );
        int w = view[2];
        int h = view[3];
        
        // get pixels
        Image image( w, h );
        glReadPixels( 0, 0, w, h, GL_BGR, GL_FLOAT, &image(0,0) );
        
        stringstream filename;
        filename << "frames/viewer" << setw(8) << setfill( '0' ) << index << ".tga";
        image.write( filename.str().c_str() );
        
        index++;
    }

    //====================================================display====================================================//
    void Viewer::display(void) {
        glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
        shader.enable();
        
        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();
        GLint viewport[4];
        glGetIntegerv( GL_VIEWPORT, viewport );
        double aspect = (double) viewport[2] / (double) viewport[3];
        const double fovy = 50.;
        const double clipNear = .01;
        const double clipFar = 1000.;
        gluPerspective( fovy, aspect, clipNear, clipFar );
        
        glMatrixMode( GL_MODELVIEW );
        glLoadIdentity();
        
        Quaternion    eye = Vector3D( 0., 0., -2.5*camera.zoom );
        Quaternion center = Vector3D( 0., 0., 0. );
        Quaternion     up = Vector3D( 0., 1., 0. );
        gluLookAt(   eye[1],    eye[2],    eye[3],
                  center[1], center[2], center[3],
                  up[1],     up[2],     up[3] );
        
        
        Quaternion r = camera.currentRotation();
        eye = r.conj() * eye * r;
        GLint uniformEye = glGetUniformLocation( shader, "eye" );
        glUniform3f( uniformEye, eye[1], eye[2], eye[3] );
        
        Quaternion light = Vector3D( -1., 1., -2. );
        light = r.conj() * light * r;
        GLint uniformLight = glGetUniformLocation( shader, "light" );
        glUniform3f( uniformLight, light[1], light[2], light[3] );
        
        camera.setView();
        callDisplayList();
        shader.disable();
        glutSwapBuffers();
    }

    //=======================================================updateDisplayList======================================//
    void Viewer::updateDisplayList(void) {

        if (surfaceDL) {
            glDeleteLists( surfaceDL, 1 );
            surfaceDL = 0;
        }
        
        surfaceDL = glGenLists( 1 );
        glNewList( surfaceDL, GL_COMPILE );
        setMeshMaterial();
        drawScene();
        glEndList();
    }

    //======================================================setGL====================================================//
    void Viewer::setGL(void) {
        glClearColor( .5, .5, .5, 1. );
        setLighting();
    }

    //=====================================================setLighting===============================================//
    void Viewer::setLighting(void) {
        GLfloat position[4] = {20., 30., 40., 0.}; // position of light source
        glLightfv( GL_LIGHT0, GL_POSITION, position );
        glEnable( GL_LIGHT0 );
        glEnable( GL_NORMALIZE );
    }

    //=====================================================setMeshMaterial==========================================//
    void Viewer::setMeshMaterial(void) {
        GLfloat  diffuse[4] = { .8, .5, .3, 1. };
        GLfloat specular[4] = { .3, .3, .3, 1. };
        GLfloat  ambient[4] = { .2, .2, .5, 1. };
        
        glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE,   diffuse  );
        glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR,  specular );
        glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT,   ambient  );
        glMaterialf ( GL_FRONT_AND_BACK, GL_SHININESS, 16.      );
    }

    //=====================================================callDisplayList==========================================//
    void Viewer::callDisplayList(void) {
        glPushAttrib( GL_ALL_ATTRIB_BITS );
        glEnable( GL_DEPTH_TEST );
        glEnable( GL_LIGHTING );
        glCallList( surfaceDL );
        glPopAttrib();
    }

    //=====================================================drawScene================================================//
    void Viewer::drawScene(void) {

        glPushAttrib( GL_ALL_ATTRIB_BITS );
        
        if (renderPolygons) {
            glEnable(GL_POLYGON_OFFSET_FILL);
            glPolygonOffset(1., 1.);
            glColor3d(1., .5, .25);
            drawPolygons();
            glDisable(GL_POLYGON_OFFSET_FILL);
        }

        if (renderBoundingBox) {
            BoundingBox box = *(octree.getBoundingBox());
            drawBondingBox(box);
        }
        
        if( renderWireframe ) drawWireframe();
        
        if (renderOctree) drawOctree();
        
        glPopAttrib();
    }

    //===================================================drawPolygons================================================//
    void Viewer::drawPolygons(void) {

        for (FaceCIter f = mesh.faces.begin(); f != mesh.faces.end(); f++) {

            if( f->isBoundary() ) continue;
            
            glBegin( GL_POLYGON );
            if (renderWireframe) {
                Vector3D N = f->normal();
                glNormal3dv( &N[0] );
            }
            
            HalfEdgeCIter he = f->he;
            do {
                if (not renderWireframe) {
                    Vector3D N = he->vertex->normal();
                    glNormal3dv( &N[0] );
                }
                
                glVertex3dv( &he->vertex->position[0] );
                
                he = he->next;
            }
            while( he != f->he );
            glEnd();
        }
    }

    //================================================drawOctree=====================================================//
    void Viewer :: drawOctree( void )
    {
        shader.disable();
        glPushAttrib( GL_ALL_ATTRIB_BITS );
        
        glDisable( GL_LIGHTING );
        glColor4f( 0., 0., 0., 0.5 );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );
        
        // Get the absolute center of octree
        Vector3D absoluteCenter = octree.center();

        // (0.5, 0.5)
        Vector3D relativeRootCenter = octree.rootNode()->center();
        
        double scale = octree.scale();
        
        // Iterate the octree cell
        std::stack<OctreeCell*> collections;
        collections.push(octree.rootNode());
        
        while(!collections.empty())
        {
            
            OctreeCell* currentNode = reinterpret_cast<OctreeCell*>(collections.top());
            collections.pop();
            
            // Draw the bounding box for current node
            BoundingBox bindBox;
            
            // The relative center based on current node, range is [0,1]
            Vector3D relativeOffset = currentNode->center()-relativeRootCenter;
            Vector3D center = absoluteCenter + relativeOffset*scale;
            
            double width = currentNode->width()*scale;
            double d2Width = width / 2;
            
            bindBox.origin.x = center.x - d2Width;
            bindBox.origin.y = center.y - d2Width;
            bindBox.origin.z = center.z - d2Width;
            bindBox.size.x = bindBox.size.y = bindBox.size.z = width;

            if (currentNode->DB_flag)
            drawBondingBox(bindBox);
            
            // Push the childrens to stack
            if (!currentNode->isLeaf()) {
                for (int i = 0; i < 8; i++) {
                    collections.push(currentNode->getChildren(i));
                }
            }
        }
        
        glPopAttrib();
    }

    //=======================================================drawBoundingBox==========================================//
    void Viewer::drawBondingBox(BoundingBox box) {

        glBegin(GL_LINES);
        
        //1
        glVertex3d(box.origin.x, box.origin.y, box.origin.z);
        glVertex3d(box.origin.x, box.origin.y + box.size.y, box.origin.z);
        
        glVertex3d(box.origin.x, box.origin.y + box.size.y, box.origin.z);
        glVertex3d(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z);
        
        glVertex3d(box.origin.x, box.origin.y + box.size.y, box.origin.z);
        glVertex3d(box.origin.x, box.origin.y + box.size.y, box.origin.z + box.size.z);
        
        //2
        glVertex3d(box.origin.x + box.size.x, box.origin.y, box.origin.z);
        glVertex3d(box.origin.x, box.origin.y, box.origin.z);
        
        glVertex3d(box.origin.x + box.size.x, box.origin.y, box.origin.z);
        glVertex3d(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z);
        
        glVertex3d(box.origin.x + box.size.x, box.origin.y, box.origin.z);
        glVertex3d(box.origin.x + box.size.x, box.origin.y, box.origin.z + box.size.z);
        
        //3
        glVertex3d(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z + box.size.z);
        glVertex3d(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z);
        
        glVertex3d(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z + box.size.z);
        glVertex3d(box.origin.x + box.size.x, box.origin.y, box.origin.z + box.size.z);
        
        glVertex3d(box.origin.x + box.size.x, box.origin.y + box.size.y, box.origin.z + box.size.z);
        glVertex3d(box.origin.x, box.origin.y + box.size.y, box.origin.z + box.size.z);
        
        //4
        glVertex3d(box.origin.x, box.origin.y, box.origin.z + box.size.z);
        glVertex3d(box.origin.x, box.origin.y, box.origin.z);
        
        glVertex3d(box.origin.x, box.origin.y, box.origin.z + box.size.z);
        glVertex3d(box.origin.x + box.size.x, box.origin.y, box.origin.z + box.size.z);
        
        glVertex3d(box.origin.x, box.origin.y, box.origin.z + box.size.z);
        glVertex3d(box.origin.x, box.origin.y + box.size.y, box.origin.z + box.size.z);
        
        glEnd();
    }

    //======================================================drawWireframe=============================================//
    void Viewer::drawWireframe(void) {

        shader.disable();
        glPushAttrib( GL_ALL_ATTRIB_BITS );
        
        glDisable( GL_LIGHTING );
        glColor4f( 0., 0., 0., 0.5 );
        glEnable( GL_BLEND );
        glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

        glBegin(GL_LINES);  // tranvese all the edges
        for( EdgeCIter e  = mesh.edges.begin();
            e != mesh.edges.end();
             e++) {
            glVertex3dv( &e->he->vertex->position[0] );
            glVertex3dv( &e->he->flip->vertex->position[0] );
        }
        glEnd();
        
        glPopAttrib();
    }

    //======================================================drawIsolatedVertices=======================================//
    void Viewer::drawIsolatedVertices(void) {
        glPushAttrib( GL_ALL_ATTRIB_BITS );
        
        glPointSize( 5 );
        glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
        glEnable( GL_POINT_SMOOTH );
        glColor3f( 1., 0., 0. );
        
        glBegin( GL_POINTS );
        for( VertexCIter v  = mesh.vertices.begin();
            v != mesh.vertices.end();
            v ++ )
        {
            if( v->isIsolated() )
            {
                glVertex3dv( &v->position[0] );
            }
        }
        glEnd();
        
        glPopAttrib();
    }

    //=====================================================drawVertices==============================================//
    void Viewer :: drawVertices( void )
    {
        for( VertexCIter v = mesh.vertices.begin();
            v != mesh.vertices.end();
            v ++ )
        {
            glLoadName(v->index);
            glBegin(GL_POINTS);
            glVertex3dv( &v->position[0] );
            glEnd();
        }
    }

    //======================================================drawSelecetdVertices=====================================//
    void Viewer::drawSelectedVertices(void) {
        shader.disable();
        glPushAttrib( GL_ALL_ATTRIB_BITS );
        
        glEnable(GL_COLOR_MATERIAL);
        glHint( GL_POINT_SMOOTH_HINT, GL_NICEST );
        glEnable( GL_POINT_SMOOTH );
        glColor3f( 0., 0., 0.5 );
        glPointSize( 20 );
        
        glBegin(GL_POINTS);
        for( VertexCIter v = mesh.vertices.begin();
            v != mesh.vertices.end();
            v ++ )
        {
            if( v->tag ) glVertex3dv( &v->position[0] );
        }
        glEnd();
        
        glPopAttrib();
    }

    //=====================================================pickVertex================================================//
    void Viewer::pickVertex(int x, int y) {
        int width  = glutGet(GLUT_WINDOW_WIDTH );
        int height = glutGet(GLUT_WINDOW_HEIGHT);
        if( x < 0 || x >= width || y < 0 || y >= height ) return;
        
        int bufSize = mesh.vertices.size();
        GLuint* buf = new GLuint[bufSize];
        glSelectBuffer(bufSize, buf);
        
        GLint viewport[4];
        GLdouble projection[16];
        glGetIntegerv( GL_VIEWPORT, viewport );
        glGetDoublev(GL_PROJECTION_MATRIX, projection);
        
        glRenderMode(GL_SELECT);
        glInitNames();
        glPushName(0);
        
        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluPickMatrix(x, viewport[3]-y, 10, 10, viewport);
        glMultMatrixd(projection);
        
        glMatrixMode(GL_MODELVIEW);
        glPushMatrix();
        drawVertices();
        glPopMatrix();
        
        glMatrixMode(GL_PROJECTION);
        glPopMatrix();
        
        glMatrixMode(GL_MODELVIEW);
        long hits = glRenderMode(GL_RENDER);
        
        int index = -1;
        double min_z = 1.0e100;
        for( long i = 0; i < hits; ++i )
        {
            double distance = buf[4*i + 1];
            if( distance < min_z )
            {
                index = buf[4*i + 3];
                min_z = distance;
            }
        }
        delete[] buf;
        
        if (index >= 0)
        {
            mesh.vertices[index].toggleTag();
            updateDisplayList();
        }
    }
    
    void Viewer::mPolygons(void) {
        renderPolygons = !renderPolygons;
        updateDisplayList();
    }
}

