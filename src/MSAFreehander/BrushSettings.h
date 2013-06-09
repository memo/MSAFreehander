#pragma once

#include "ofMain.h"
#include "ofxXmlSettings.h"

namespace msa {
    namespace freehander {
        
        class BrushSettings {
        public:
            ofColor color;
            float thickness;
            int smoothAmount;   // how much to smooth the path (zero to ignore)
            float optimizeTolerance;    // howmuch to optimize the path, based on curvature (zero to ignore)
            
            //--------------------------------------------------------------
            BrushSettings() {
                color.set(0, 0, 0, 255);
                thickness = 3;
                smoothAmount = 2;
                optimizeTolerance = 0;
            }
            
            //--------------------------------------------------------------
            string getString() const {
                stringstream s;
                s << "BrushSettings:" << endl;
                s << "color : " << color << endl;
                s << "thickness : " << thickness << endl;
                s << "smoothAmount : " << smoothAmount << endl;
                s << "optimizeTolerance : " << optimizeTolerance << endl;
                return s.str();
            }
            
            //--------------------------------------------------------------
            void writeToXml(ofxXmlSettings &xml) const {
                xml.addValue("color", color.getHex());
                xml.addValue("thickness", thickness);
                xml.addValue("smoothAmount", smoothAmount);
                xml.addValue("optimizeTolerance", optimizeTolerance);
            }
            
            //--------------------------------------------------------------
            void readFromXml(ofxXmlSettings &xml) {
                color.setHex(xml.getValue("color", 0));
                thickness = xml.getValue("thickness", 3.0f);
                smoothAmount = xml.getValue("smoothAmount", 2);
                optimizeTolerance = xml.getValue("optimizeTolerance", 0.0f);
            }
        };
        
    }
}