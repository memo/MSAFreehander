#pragma once

#include "ofxMSAInteractiveObject.h"
#include "ModeManager.h"
#include "ShapeManager.h"
#include "ViewTransformer.h"

namespace msa {
    namespace freehander {
        
        class OutputView : public ofxMSAInteractiveObject {
        public:
            ofColor bgColor;
            ShapeManager &shapeManager;
            ViewTransformer &viewTransformer;
            
            OutputView(ShapeManager &shapeManager, ViewTransformer &viewTransformer, ofColor bgColor)
            : shapeManager(shapeManager), viewTransformer(viewTransformer), bgColor(bgColor) {
                reset();
            }
            
            //--------------------------------------------------------------
            void reset() {
                panSpeed.set(0, 0);
                zoomSpeed = 0;
            }
            
            //--------------------------------------------------------------
            virtual void onPress(int x, int y, int button) {
                zoomSpeed = 0;
                
                ofVec2f p(viewTransformer.fromDst(ofVec2f(x, y)));
                
                switch(ModeManager::instance().getMode()) {
                    case ModeManager::kDraw:
                        shapeManager.clearSelection();
                        shapeManager.addVertex(p, false);
                        break;
                        
                    case ModeManager::kConnectLines:
                        shapeManager.addVertex(p, false);
                        break;
                        
                    case ModeManager::kText:
                        shapeManager.addText(p);
                        break;
                        
                    case ModeManager::kSelect:
                        shapeManager.selectShapes(p, 10.0f / viewTransformer.getScale(), true);  // XML
                        bClickOnShape = shapeManager.hasSelection();
                        break;
                        
                        
                    default:
                        break;
                }
            }
            
            //--------------------------------------------------------------
            virtual void onDragOver(int x, int y, int button) {
                if(!isMousePressed()) return;   // check if mouse was pressed over this window (instead of being dragged in from outside)
                
                ofVec2f p(viewTransformer.fromDst(ofVec2f(x, y)));
                ofVec2f mouseDiff(ofGetMouseX() - ofGetPreviousMouseX(), ofGetMouseY() - ofGetPreviousMouseY());
                ofVec2f mouseDiffScaled(mouseDiff / viewTransformer.getScale());
                
                
                switch(ModeManager::instance().getMode()) {
                    case ModeManager::kDraw:
                    case ModeManager::kConnectLines:
                        shapeManager.addVertex(p, false);
                        break;
                        
                    case ModeManager::kPan:
                        panSpeed -= 0.5 * mouseDiffScaled;
                        break;
                        
                    case ModeManager::kZoom:
                        zoomSpeed -=
                        mouseDiff.x/viewTransformer.getDstRect().width -
                        mouseDiff.y/viewTransformer.getDstRect().height;
                        break;
                        
                    case ModeManager::kSelect:
                        if(bClickOnShape) shapeManager.moveBy(mouseDiffScaled);
                        else shapeManager.selectShapes(p, 10.0f / viewTransformer.getScale(), false);  // XML
                        break;
                        
                        
                    default:
                        break;
                        
                }
            }
            
            //--------------------------------------------------------------
            virtual void onDragOutside(int x, int y, int button) {
                onRelease(x, y, button);
            }
            
            //--------------------------------------------------------------
            virtual void onRelease(int x, int y, int button) {
                switch(ModeManager::instance().getMode()) {
                    case ModeManager::kDraw:
                        shapeManager.clearSelection();
                        break;
                        
                    default:
                        break;
                }
            }
            
            //--------------------------------------------------------------
            void update() {
                if(panSpeed.lengthSquared() > FLT_EPSILON) {
                    viewTransformer.getSrcRect().translate(panSpeed);
                    if(isMousePressed()) panSpeed *= 0.5;
                    else panSpeed *= 0.95;
                    if(panSpeed.lengthSquared() < FLT_EPSILON) {
                        //                        ofLogVerbose() << "OutputView::panSpeed settling to zero";
                        panSpeed.set(0, 0);
                    }
                }
                
                if(fabsf(zoomSpeed) > 1e-3) {
                    viewTransformer.getSrcRect().scaleFromCenter(1 + zoomSpeed);
                    if(isMousePressed()) zoomSpeed *= 0.5;
                    else zoomSpeed *= 0.95;
                    if(fabsf(zoomSpeed) < 1e-3) {
                        //                        ofLogVerbose() << "OutputView::zoomSpeed settling to zero";
                        zoomSpeed = 0;
                    }
                }
                
            }
            
            
            //--------------------------------------------------------------
            void draw(bool doDrawGrid, int gridSize) {
                ofPushStyle();
                ofFill();
                ofSetColor(bgColor);
                ofRect((ofRectangle)*this);
                ofNoFill();
                ofSetColor(ofColor::black);
                ofRect((ofRectangle)*this);
                
                ofEnableAlphaBlending();
                ofEnableSmoothing();
                
                
                ofPushMatrix();
                ofTranslate(x, y);
                ofPopMatrix();
                
                viewTransformer.begin();
                shapeManager.draw(doDrawGrid, gridSize, viewTransformer.getSrcRect());
                viewTransformer.end();
                
                ofPopStyle();
            }
            
            
        private:
            ofVec2f panSpeed;
            float zoomSpeed;
            bool bClickOnShape;
        };
        
    }
}