#pragma once

#include "ofxMSAInteractiveObject.h"
#include "ModeManager.h"
#include "ShapeManager.h"
#include "ViewTransformer.h"

namespace msa {
    namespace freehander {
        
        class MapView : public ofxMSAInteractiveObject {
        public:
            ofColor bgColor;
            ViewTransformer &viewTransformer;
            
            MapView(ViewTransformer &viewTransformer, ofColor bgColor)
            : viewTransformer(viewTransformer), bgColor(bgColor) {
                reset();
            }
            
            //--------------------------------------------------------------
            void reset() {
                width = 0;
                height = 0;
            }

            //--------------------------------------------------------------
            virtual void onPress(int x, int y, int button) {
                viewTransformer.getSrcRect().setFromCenter(
                                                           ofMap(x, getLeft(), getRight(), inputFullBounds.getLeft(), inputFullBounds.getRight()),
                                                           ofMap(y, getTop(), getBottom(), inputFullBounds.getTop(), inputFullBounds.getBottom()),
                                                           viewTransformer.getSrcRect().width,
                                                           viewTransformer.getSrcRect().height);
            }

            
            //--------------------------------------------------------------
            virtual void onDragOver(int x, int y, int button) {
                if(!isMousePressed()) return;   // check if mouse was pressed over this window (instead of being dragged in from outside)
                
                switch(ModeManager::instance().getMode()) {
                    case ModeManager::kZoom: {
                        ofVec2f center(viewTransformer.getSrcRect().getCenter());
                        ofVec2f p(
                                  ofMap(x, getLeft(), getRight(), inputFullBounds.getLeft(), inputFullBounds.getRight()),
                                  ofMap(y, getTop(), getBottom(), inputFullBounds.getTop(), inputFullBounds.getBottom()));
                        p -= center;
                        p *= 2;
                        viewTransformer.getSrcRect().setFromCenter(center, p.x, p.y);

                    }
                        break;
                        
                    default:
                        onPress(x, y, button);
                        break;
                }
            }
            
            //--------------------------------------------------------------
            virtual void draw(ShapeManager &shapeManager) {
                ofPushStyle();
                
                // draw bg
                ofFill();
                ofSetColor(bgColor);
                ofRect((ofRectangle)*this);
                
                // draw frame
                ofNoFill();
                ofSetColor(ofColor::black);
                ofRect((ofRectangle)*this);
                
                inputFullBounds = shapeManager.getBoundingBox();
                
                ofVec2f bsize(inputFullBounds.getWidth(), inputFullBounds.getHeight());
                if(bsize.lengthSquared() > 0) {
                    ofPushStyle();
                    ofPushMatrix();
                    float targetSize = 256; // XML
                    float scale = (bsize.x < bsize.y) ? targetSize / bsize.y : targetSize / bsize.x;
                    width = inputFullBounds.getWidth() * scale;
                    height = inputFullBounds.getHeight() * scale;
                    ofScale(scale, scale, 1);
                    ofTranslate(-inputFullBounds.getPosition());
                    
                    shapeManager.draw(0, 0);
                    
                    // draw view
                    ofNoFill();
                    ofSetColor(0, 255, 0);
                    ofRect(viewTransformer.getSrcRect().getIntersection(ofRectangle(inputFullBounds)));
                    ofPopMatrix();
                    ofPopStyle();
                }
                
                ofPopStyle();
            }
            
            
        protected:
            ofRectangle inputFullBounds;
        };
        
    }
}