#pragma once

#include "ShapeBase.h"
#include "FontManager.h"

namespace msa {
    namespace freehander {
        class ShapeText : public ShapeBase {
        public:
            string str;
            //            shared_ptr<ofTrueTypeFont> fontPtr;
            
            
            //--------------------------------------------------------------
            ShapeText(ofVec2f pos = ofVec2f()) : ShapeBase(pos) {
                str = "";
            }
            
            //--------------------------------------------------------------
            void addKey(int key) {
                switch(key) {
                    case OF_KEY_BACKSPACE:
                        deleteLastElement();
                        break;
                        
                    default:
                        str += key;
                }
                
            }
            
            //--------------------------------------------------------------
            virtual void clear() {
                str = "";
            }
            

            //--------------------------------------------------------------
            virtual int size() const {
                return str.size();
            }
            
            //--------------------------------------------------------------
            virtual void deleteLastElement() {
                if(str.size()>0) str.resize(str.size()-1);
            }
            
            //--------------------------------------------------------------
            virtual bool shouldDelete() const {
                return str.empty();
            }
            
            //--------------------------------------------------------------
            virtual void draw(bool selected) {
                ofPushStyle();

                string s = str;
                if(selected) {
                    if(ModeManager::instance().isMode(ModeManager::kText)) {
                       if(((int)(ofGetElapsedTimef()*2))%2 == 0) s += "_";
                    }

                    // bit of a hack
//                    ModeManager::instance().setMode(ModeManager::kText);
                }
                ofFill();
                ofSetColor(0);
                FontManager::instance().getFont("main").drawStringAsShapes(s, getX(), getY());
                ofPopStyle();
            }
            
            virtual string getType() {
                return "text";
            }

            
            //--------------------------------------------------------------
            virtual ofRectangle getBoundingBox() {
                return FontManager::instance().getFont("main").getStringBoundingBox(str + "Q", getX(), getY());
            }
            
            //--------------------------------------------------------------
            virtual bool hitTest(ofVec2f point, float distanceThreshold) {
                return getBoundingBox().inside(point);
            }
            
            //--------------------------------------------------------------
            virtual void writeToXml(ofxXmlSettings &xml) const {
                xml.addValue("string", str);
            }
            
            //--------------------------------------------------------------
            virtual void readFromXml(ofxXmlSettings &xml) {
                clear();
                str = xml.getValue("string", "unknown string");
            }

        };
    }
}