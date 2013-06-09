#pragma once

#include "ofMain.h"
#include "BrushSettings.h"
#include "ShapeBase.h"

namespace msa {
    namespace freehander {
        
        class ShapeVector : public ShapeBase {
        public:
            BrushSettings brushSettings;
            
            ShapeVector(ofVec2f pos = ofVec2f(), BrushSettings brushSettings = BrushSettings());
            void addVertex(ofVec2f v);
            
            virtual void clear();
            virtual int size() const;
            virtual void deleteLastElement();
            virtual bool shouldDelete() const;
            virtual void draw(bool selected);
            virtual ofRectangle getBoundingBox();
            virtual bool hitTest(ofVec2f point, float distThreshold);
            virtual string getType();
            virtual void writeToXml(ofxXmlSettings &xml) const;
            virtual void readFromXml(ofxXmlSettings &xml);
            
        protected:
            ofPolyline origPoly;
            ofPolyline processedPoly;
            
            ofPolyline &getActivePoly();
            
        private:
            void update();
            
        };
        
    }
}