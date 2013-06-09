#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"
#include "BrushSettings.h"

namespace msa {
    namespace freehander {
        
        class ShapeBase : public ofNode {
        public:
            
            ShapeBase(ofVec2f pos) { setPosition(pos); }
            
            virtual void clear() = 0;
            virtual int size() const = 0;
            virtual void deleteLastElement() = 0;
            virtual bool shouldDelete() const = 0;
            virtual ofRectangle getBoundingBox() = 0;
            virtual bool hitTest(ofVec2f point, float distanceThreshold) = 0;
            virtual void draw(bool selected) = 0;
            virtual string getType() = 0;
            virtual void writeToXml(ofxXmlSettings &xml) const = 0;
            virtual void readFromXml(ofxXmlSettings &xml) = 0;
        };
        
        typedef shared_ptr<ShapeBase> ShapeBasePtr;
    }
}