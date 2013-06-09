
#pragma once

#include "ofMain.h"

namespace msa {
    namespace freehander {
        
        class ViewTransformer {
        public:
            //--------------------------------------------------------------
            ViewTransformer() {
                _scale = 0;
                _bDirty = true;
            }
            
            //--------------------------------------------------------------
            string getString() const {
                update();
                stringstream s;
                s << "ViewTransformer:" << endl;
                s << "scale : " << getScale() << endl;
                s << "srcRect : " << endl << " " << getSrcRect().getTopLeft() << endl << " " << getSrcRect().getWidth() << " x " << getSrcRect().getHeight() << endl;
                s << "dstRect : " << endl << " " << getDstRect().getTopLeft() << endl << " " << getDstRect().getWidth() << " x " << getDstRect().getHeight() << endl;
                return s.str();
            }
            
            //--------------------------------------------------------------
            float getScale() const {
                update();
                return _scale;
            }
            
            //--------------------------------------------------------------
            ofRectangle getSrcRect() const {
                update();
                return _srcRect;
            }
            
            //--------------------------------------------------------------
            ofRectangle getDstRect() const {
                update();
                return _dstRect;
            }
            
            //--------------------------------------------------------------
            ofRectangle &getSrcRect() {
                _bDirty = true;
                return _srcRect;
            }
            
            //--------------------------------------------------------------
            ofRectangle &getDstRect() {
                _bDirty = true;
                return _dstRect;
            }


            //--------------------------------------------------------------
            // convert from destination (i.e. output window) coordinates to world coordinates
            ofVec2f fromDst(ofVec2f p) const {
                p.x = ofMap(p.x, getDstRect().getLeft(), getDstRect().getRight(), getSrcRect().getLeft(), getSrcRect().getRight());
                p.y = ofMap(p.y, getDstRect().getTop(), getDstRect().getBottom(), getSrcRect().getTop(), getSrcRect().getBottom());
                return p;
            }
            
            //--------------------------------------------------------------
            ofVec2f toDst(ofVec2f p) const {
                p.x = ofMap(p.x, getSrcRect().getLeft(), getSrcRect().getRight(), getDstRect().getLeft(), getDstRect().getRight());
                p.y = ofMap(p.y, getSrcRect().getTop(), getSrcRect().getBottom(), getDstRect().getTop(), getDstRect().getBottom());
                return p;
            }
            
            //--------------------------------------------------------------
            void begin() const {
                ofPushMatrix();
                ofTranslate(getDstRect().getPosition());
                ofScale(_scale, _scale, 1);
                ofTranslate(-getSrcRect().getPosition());
            }
            
            //--------------------------------------------------------------
            void end() const {
                ofPopMatrix();
            }
            
        protected:
            mutable float _scale;
            mutable bool _bDirty;
            mutable ofRectangle _srcRect, _dstRect;
            
            //--------------------------------------------------------------
            void update() const {
                if(_bDirty && _srcRect.width > 0 && _srcRect.height > 0) {
                    _bDirty = false;
                    _scale = MIN(fabs(_dstRect.width / _srcRect.width), fabsf(_dstRect.height / _srcRect.height));
                    _srcRect.setFromCenter(_srcRect.getCenter(), _dstRect.width / _scale, _dstRect.height / _scale);
                }
                _srcRect.standardize();
                _dstRect.standardize();
            }
            

            
        };
        
    }
}