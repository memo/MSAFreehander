//
//  ShapeVector.cpp
//  MSAFreehander
//
//  Created by Memo Akten on 29/05/2013.
//
//

#include "ShapeVector.h"

namespace msa {
    namespace freehander {
        
        //--------------------------------------------------------------
        ShapeVector::ShapeVector(ofVec2f pos, BrushSettings brushSettings) : ShapeBase(pos), brushSettings(brushSettings) {
        }
        
        //--------------------------------------------------------------
        void ShapeVector::addVertex(ofVec2f v) {
            origPoly.addVertex(v);
            update();
        }
        
        
        //--------------------------------------------------------------
        ofPolyline& ShapeVector::getActivePoly() {
            return processedPoly;
        }
        
        
#pragma mark ShapeBase
        
        
        //--------------------------------------------------------------
        void ShapeVector::clear() {
            origPoly.clear();
            processedPoly.clear();
            //            update();
        }
        
        //--------------------------------------------------------------
        int ShapeVector::size() const {
            return origPoly.size();
        }
        
        //--------------------------------------------------------------
        void ShapeVector::deleteLastElement() {
            if(origPoly.size() > 0) {
                origPoly.getVertices().pop_back();
                update();
            }
        }
        
        //--------------------------------------------------------------
        bool ShapeVector::shouldDelete() const {
            return origPoly.size() <= 1;
        }
        
        //--------------------------------------------------------------
        ofRectangle ShapeVector::getBoundingBox() {
            return getActivePoly().getBoundingBox() + getPosition();
        }
        
        //--------------------------------------------------------------
        bool ShapeVector::hitTest(ofVec2f point, float distThreshold) {
            if(getBoundingBox().inside(point)) {
                point -= getPosition(); // hack?
                ofVec2f closestPoint = getActivePoly().getClosestPoint(point);
                if(point.distanceSquared(closestPoint) < distThreshold * distThreshold) return true;
            }
            return false;
        }
        
        //--------------------------------------------------------------
        void ShapeVector::draw(bool selected) {
            //            if(origPoly.hasChanged()) update();
            
            ofPushStyle();
            transformGL();
            //            if(selected) {
            //                ofSetLineWidth(brushSettings.thickness + 3);
            //                ofSetColor(brushSettings.color, 100);
            //                getActivePoly().draw();
            //            }
            ofSetLineWidth(brushSettings.thickness);
            ofSetColor(brushSettings.color);
            getActivePoly().draw();
            restoreTransformGL();
            ofPopStyle();
        }
        
        //--------------------------------------------------------------
        string ShapeVector::getType() {
            return "vector";
        }
        
        //--------------------------------------------------------------
        void ShapeVector::update() {
            if(brushSettings.smoothAmount > 0) processedPoly = origPoly.getSmoothed(brushSettings.smoothAmount);
            else processedPoly = origPoly;
            if(brushSettings.optimizeTolerance > 0) processedPoly.simplify(brushSettings.optimizeTolerance);
        }
        
        //--------------------------------------------------------------
        void ShapeVector::writeToXml(ofxXmlSettings &xml) const {
            xml.addTag("settings");
            xml.pushTag("settings");
            brushSettings.writeToXml(xml);
            xml.popTag();
            
            xml.addTag("points");
            xml.pushTag("points");
            for(int i=0; i<origPoly.size(); i++) {
                ofVec2f p(origPoly[i]);
                int id = xml.addTag("point");
                xml.addAttribute("point", "x", p.x, id);
                xml.addAttribute("point", "y", p.y, id);
                //                xml.addAttribute("point", "z", p.z, id);
            }
            xml.popTag();
        }
        
        //--------------------------------------------------------------
        void ShapeVector::readFromXml(ofxXmlSettings &xml) {
            clear();
            xml.pushTag("settings");
            brushSettings.readFromXml(xml);
            xml.popTag();
            
            xml.pushTag("points");
            int numPoints = xml.getNumTags("point");
            for(int i=0; i<numPoints; i++) {
                ofVec2f p;
                p.x = xml.getAttribute("point", "x", ofRandomuf(), i);
                p.y = xml.getAttribute("point", "y", ofRandomuf(), i);
                //                p.z = xml.getAttribute("point", "z", ofRandomuf(), i);
                addVertex(p);
            }
            xml.popTag();
        }
        
        
    }
}