#include "testApp.h"

#include "MSAFreehander/FontManager.h"
#include "MSAFreehander/ShapeManager.h"
#include "MSAFreehander/ViewTransformer.h"
#include "MSAFreehander/ModeManager.h"

#include "MSAFreehander/OutputView.h"
#include "MSAFreehander/MapView.h"

#include "glut/glut.h"

using namespace msa;
using namespace freehander;


ShapeManager shapeManager;
ViewTransformer viewTransformer;

OutputView outputView(shapeManager, viewTransformer, ofColor(255, 255, 255, 255));
MapView mapView(viewTransformer, ofColor(255, 255, 255, 255));

bool doSaveAsPDF = false;

bool doDrawGrid = true;
int gridSize = 10;



//--------------------------------------------------------------
void reset() {
    shapeManager.reset();
    mapView.reset();
    viewTransformer.getSrcRect() = outputView;
}

//--------------------------------------------------------------
void testApp::setup(){
    ofSetEscapeQuitsApp(false);
    
    FontManager::instance().loadFont("main", "fonts/verdana.ttf", 16, true, true, true);
    
    outputView.disableAllEvents();
    outputView.enableMouseEvents();
    
    mapView.disableAllEvents();
    mapView.enableMouseEvents();
    
    outputView.set(256, 0, ofGetWidth()-256, ofGetHeight());
    reset();
}


//--------------------------------------------------------------
void testApp::update() {
    outputView.set(256, 0, ofGetWidth()-256, ofGetHeight());
    viewTransformer.getDstRect() = outputView;
    outputView.update();
}

//--------------------------------------------------------------
void testApp::draw(){
    ofEnableAlphaBlending();
    ofEnableSmoothing();
    
    mapView.draw(shapeManager);
    
    glEnable(GL_SCISSOR_TEST);  // TODO: is there a better or OF way of doing this?
    glScissor(outputView.x, outputView.y, outputView.width, outputView.height);
    outputView.draw(doDrawGrid, gridSize);
    glDisable(GL_SCISSOR_TEST);
    
    ofPushStyle();
    ofNoFill();
    ofSetColor(shapeManager.brushSettings.color);
    ofCircle(ofGetMouseX(), ofGetMouseY(), shapeManager.brushSettings.thickness/2);
    ofPopStyle();
    
    if(doSaveAsPDF) {
        doSaveAsPDF = false;
        ofRectangle r = shapeManager.getBoundingBox();
        ofFbo fbo;
        fbo.allocate(r.width, r.height);
        fbo.begin();
        ofClear(255);
        ofPushMatrix();
        ofTranslate((int)-r.x, (int)-r.y);
        shapeManager.draw(false);
        ofPopMatrix();
        fbo.end();
        
        ofImage img;
        fbo.readToPixels(img.getPixelsRef());
        img.saveImage("Export/" + ofToString(ofGetTimestampString()) + ".png");
        
        //        ofBeginSaveScreenAsPDF("Export/" + ofToString(ofGetTimestampString()) + ".pdf", false, false, r);
        //        ofFill();
        //        ofSetColor(outputView.bgColor);
        //        ofRect(shapeManager.getBoundingBox());
        //        shapeManager.draw();
        //        ofEndSaveScreenAsPDF();
    }
    
    stringstream s;
    s << "MSAFreehander v0.1 " << "\n";
    s << ofGetTimestampString() << "\n";
    s << "\n";
    s << shapeManager.getString() << "\n\n";
    s << viewTransformer.getString() << "\n\n";
    s << ModeManager::instance().getString() << "\n\n";
    s << ofGetFrameRate() << "\n\n";
    ofSetColor(0);
    ofDrawBitmapString(s.str(), 10, 350);
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    //    ofLogNotice() << key;
    if(ModeManager::instance().isMode(ModeManager::kText) && shapeManager.hasSelection()) {
        shapeManager.addKey(key);
        
    } else {
        switch(key) {
                // MODES
            case 'z': ModeManager::instance().setMode(ModeManager::kZoom); break;
            case 'x': ModeManager::instance().setMode(ModeManager::kPan); break;
            case 9: ModeManager::instance().setMode(ModeManager::kConnectLines); break; // TAB
            case ' ': ModeManager::instance().toggleMode(ModeManager::kSelect); break;
                
            case 'd': ModeManager::instance().setMode(ModeManager::kDraw); break;
            case 't': ModeManager::instance().toggleMode(ModeManager::kText); break;
                
                // DISPLAY
            case 'f': ofToggleFullscreen(); break;
            case 'g': doDrawGrid ^= true; break;
            case 'e': viewTransformer.getSrcRect() = shapeManager.getBoundingBox(); break;  // zoom extents
                
                // scale
            case '.': viewTransformer.getSrcRect().scaleFromCenter(1.0/1.1); break;
            case ',': viewTransformer.getSrcRect().scaleFromCenter(1.1); break;
                
                // pan
            case OF_KEY_UP: viewTransformer.getSrcRect().translate(0, -10); break;
            case OF_KEY_DOWN: viewTransformer.getSrcRect().translate(0, 10); break;
            case OF_KEY_RIGHT: viewTransformer.getSrcRect().translate(10, 0); break;
            case OF_KEY_LEFT: viewTransformer.getSrcRect().translate(-10, 0); break;
                
            case '0': shapeManager.setColor(0, 0, 0); break;
            case '1': shapeManager.setColor(255, 0, 0); break;
            case '2': shapeManager.setColor(0, 255, 0); break;
            case '3': shapeManager.setColor(0, 0, 255); break;
            case '4': shapeManager.setColor(255, 255, 0); break;
            case '5': shapeManager.setColor(255, 0, 255); break;
            case '6': shapeManager.setColor(0, 255, 255); break;
                
                
            case 'D':
                shapeManager.duplicate();
                break;
                
            case 'N':
            {
                string s = ofSystemTextBoxDialog("Are you sure you want to start a new drawing?", "n");
                if(s.size() > 0 && s[0] == 'y') {
                    reset();   // should be CRTL+
                }
            }
                break;
                
            case OF_KEY_BACKSPACE:
                if(glutGetModifiers() & GLUT_ACTIVE_SHIFT) {
                    string s = ofSystemTextBoxDialog("Are you sure you want to delete selected items?", "n");
                    if(s.size() > 0 && s[0] == 'y') {
                        shapeManager.deleteSelectedShapes();
                    }
                    
                } else {
                    shapeManager.deleteLastElement();
                }
                break;
            case 'S': doSaveAsPDF = true; break;
                
            case 's': {
                ofFileDialogResult d = ofSystemSaveDialog("My Drawing.xml", "Save drawing");
                if(d.bSuccess) shapeManager.save(d.getPath());
            }
                break;
                
            case 'l': {
                ofFileDialogResult d = ofSystemLoadDialog("Load drawing");
                if(d.bSuccess) shapeManager.load(d.getPath());
            }
                break;
                
                
                // modify
            case ']': shapeManager.incBrushThickness(1); break;
            case '[': shapeManager.incBrushThickness(-1); break;
                
            case '}': shapeManager.incBrushSmoothing(1); break;
            case '{': shapeManager.incBrushSmoothing(-1); break;
        }
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){
    if(ModeManager::instance().isMode(ModeManager::kText) && shapeManager.hasSelection()) {
    } else {
        switch(key) {
                // modes:
            case 'z':
            case 'x':
                //            case ' ':
            case 9:
                //                shapeManager.clearSelection();
                ModeManager::instance().setMode(ModeManager::kDraw);
                break;
        }
    }
    
}
