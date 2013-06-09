//
//  ShapeManager.cpp
//  MSAFreehander
//
//  Created by Memo Akten on 29/05/2013.
//
//

#include "ShapeManager.h"


namespace msa {
    namespace freehander {
        
        
        //--------------------------------------------------------------
        void ShapeManager::reset() {
            shapes.clear();
            selectedShapes.clear();
            boundingBox.set(0, 0, 0, 0);
            memset(&num, 0, sizeof(num));
        }
        
        //--------------------------------------------------------------
        string ShapeManager::getString() const {
            stringstream s;
            s << "ShapeManager:" << endl;
            s << "num.totalShapes : " << shapes.size() << endl;
            s << "num.totalVerts : " << num.totalVerts << endl;
            s << "num.drawnShapes : " << num.drawnShapes << endl;
            s << "num.drawnVerts : " << num.drawnVerts << endl;
            s << "num.selectedShapes : " << selectedShapes.size() << endl;
            s << brushSettings.getString() << endl;
            return s.str();
        }
        
        //--------------------------------------------------------------
        ofRectangle ShapeManager::getBoundingBox() const {
            return boundingBox;
        }
        
        
        //--------------------------------------------------------------
        void ShapeManager::draw(bool doDrawGrid, int gridSize, ofRectangle drawRect) {
            ofPushStyle();
            if(doDrawGrid) {
                if(drawRect.width == 0 && drawRect.height == 0) drawRect = getBoundingBox();
                int x = floor(drawRect.x / (10 * gridSize)) * 10 * gridSize;
                int y = floor(drawRect.y / (10 * gridSize)) * 10 * gridSize;
                int w = drawRect.width + 10 * gridSize;
                int h = drawRect.height + 10 * gridSize;
                ofSetLineWidth(1);
                for(int i=x, count=0; i<x+w; i+= gridSize, count++) {
                    if(count%10 == 0) {
                        ofSetColor(100, 170, 100);
                    } else if(count%5 == 0) {
                        ofSetColor(140, 200, 140);
                    } else {
                        ofSetColor(180, 230, 180);
                    }
                    ofLine(i, y, i, y+h);
                }
                for(int j=y, count=0; j<y+h; j+= gridSize, count++) {
                    if(count%10 == 0) {
                        ofSetColor(100, 170, 100);
                    } else if(count%5 == 0) {
                        ofSetColor(140, 200, 140);
                    } else {
                        ofSetColor(180, 230, 180);
                    }
                    ofLine(x, j, x+w, j);
                }
            }
            
            
            memset(&num, 0, sizeof(num));
            int i=0;
            for(list<ShapeBasePtr>::iterator it=shapes.begin(); it!=shapes.end(); it++) {
                ShapeBase *shape = it->get();
                num.totalVerts += shape->size();
                ofRectangle shapeRect(shape->getBoundingBox());
                if((drawRect.width == 0 && drawRect.height == 0) || shapeRect.intersects(drawRect)) {
                    bool selected = selectedShapes.count(shape);
                    shape->draw(selected);
                    
                    if(selected) {
                        ofNoFill();
                        ofSetLineWidth(1);
                        ofSetColor(0, 100);
                        ofRect(shape->getBoundingBox());
                    }
                    
                    num.drawnShapes++;
                    num.drawnVerts += shape->size();
                }
                if(i==0) boundingBox = shapeRect;
                else boundingBox.growToInclude(shapeRect);
                i++;
            }
//            ofSetColor(255, 0, 0);
//            ofNoFill();
//            ofRect(boundingBox);
            ofPopStyle();
        }
        
        
        //--------------------------------------------------------------
        string ShapeManager::getVersion() {
            return "1.0.0";
        }
        
        
#pragma mark Shape modifiers
        //--------------------------------------------------------------
        void ShapeManager::addVertex(ofVec2f p, bool bCreateNew) {
            if(bCreateNew || selectedShapes.empty()) {
                addVector();
            }
            
            for(set<ShapeBase*>::iterator it=selectedShapes.begin(); it!= selectedShapes.end(); it++) {
                ShapeVector* s = dynamic_cast<ShapeVector*>(*it);
                if(s) s->addVertex(p);
            }
        }
        
        //--------------------------------------------------------------
        void ShapeManager::incBrushThickness(float inc) {
            if(selectedShapes.empty()) {
                brushSettings.thickness+=inc;
            } else {
                for(set<ShapeBase*>::iterator it=selectedShapes.begin(); it!= selectedShapes.end(); it++) {
                    ShapeVector* s = dynamic_cast<ShapeVector*>(*it);
                    if(s) s->brushSettings.thickness += inc;
                }
            }
        }
        
        //--------------------------------------------------------------
        void ShapeManager::incBrushSmoothing(float inc) {
            if(selectedShapes.empty()) {
                brushSettings.smoothAmount+=inc;
            } else {
                for(set<ShapeBase*>::iterator it=selectedShapes.begin(); it!= selectedShapes.end(); it++) {
                    ShapeVector* s = dynamic_cast<ShapeVector*>(*it);
                    if(s) s->brushSettings.smoothAmount += inc;
                }
            }
        }
        
        //--------------------------------------------------------------
        void ShapeManager::setColor(int r, int g, int b) {
            brushSettings.color.set(r, g, b);
            for(set<ShapeBase*>::iterator it=selectedShapes.begin(); it!= selectedShapes.end(); it++) {
                ShapeVector* s = dynamic_cast<ShapeVector*>(*it);
                if(s) s->brushSettings.color = brushSettings.color;
            }
        }

        //--------------------------------------------------------------
        void ShapeManager::addKey(int key) {
            if(key == OF_KEY_RETURN) {
                clearSelection();
                //                ModeManager::instance().setMode(ModeManager::kDraw);
            } else {
                for(set<ShapeBase*>::iterator it=selectedShapes.begin(); it!= selectedShapes.end(); it++) {
                    ShapeText* s = dynamic_cast<ShapeText*>(*it);
                    if(s) s->addKey(key);
                }
            }
        }
        
        
        //--------------------------------------------------------------
        void ShapeManager::moveBy(ofVec2f offset) {
            for(set<ShapeBase*>::iterator it=selectedShapes.begin(); it!= selectedShapes.end(); it++) {
                ShapeBase* s = *it;
                s->move(offset);
            }
        }
        
#pragma mark add/remove/get shapes
        
        //--------------------------------------------------------------
        void ShapeManager::deleteSelectedShapes() {
            for(set<ShapeBase*>::iterator it=selectedShapes.begin(); it!= selectedShapes.end(); it++) {
                ShapeBase *s = *it;
                s->clear();
            }
            cleanShapes();
        }
        
        //--------------------------------------------------------------
        void ShapeManager::deleteLastElement() {
            for(set<ShapeBase*>::iterator it=selectedShapes.begin(); it!= selectedShapes.end(); it++) {
                ShapeBase *s = *it;
                s->deleteLastElement();
            }
            cleanShapes();
        }
        
        //--------------------------------------------------------------
        ShapeVector* ShapeManager::addVector(ofVec2f pos) {
            addShape(new ShapeVector(pos, brushSettings));
            return dynamic_cast<ShapeVector*>(shapes.back().get());
        }
        
        //--------------------------------------------------------------
        ShapeText* ShapeManager::addText(ofVec2f pos) {
            addShape(new ShapeText(pos));
            return dynamic_cast<ShapeText*>(shapes.back().get());
        }
        
        //--------------------------------------------------------------
        ShapeBase* ShapeManager::addShape(ShapeBase *s) {
            shapes.push_back(ShapeBasePtr(s));
            //            clearSelection();
            selectedShapes.insert(s);
            return s;
        }
        
        //--------------------------------------------------------------
        void ShapeManager::cleanShapes() {
            for(list<ShapeBasePtr>::iterator it=shapes.begin(); it!=shapes.end(); ) {
                ShapeBasePtr shapePtr = *it;
                if(shapePtr->shouldDelete()) {
                    shapes.erase(it++);
                    
                    // delete from selected shapes
                    set<ShapeBase*>::iterator selectedShape = selectedShapes.find(shapePtr.get());
                    if(selectedShape != selectedShapes.end()) selectedShapes.erase(selectedShape);
                } else {
                    ++it;
                }
            }
        }
        
        
        //--------------------------------------------------------------
        void ShapeManager::duplicate() {
            set<ShapeBase*> oldSelectedShapes = selectedShapes;
            selectedShapes.clear();
            for(set<ShapeBase*>::iterator it=oldSelectedShapes.begin(); it!= oldSelectedShapes.end(); it++) {
                ShapeBase* s = *it;
                ShapeBase *newShape = NULL;
                ShapeVector *v = dynamic_cast<ShapeVector*>(s);
                if(v) newShape = addShape(new ShapeVector(*v));
                else {
                    ShapeText *t = dynamic_cast<ShapeText*>(s);
                    if(t) newShape = addShape(new ShapeText(*t));
                }
                
                if(newShape) newShape->move(10, 0, 0);  // TODO: translate in view coordinates
                
            }
            
        }
        
        
#pragma mark Selection
        //--------------------------------------------------------------
        void ShapeManager::selectShapes(ofVec2f point, float distanceThreshold, bool bClearIfNoneFound) {
            //            clearSelection();
            bool found = false;
            for(list<ShapeBasePtr>::iterator it=shapes.begin(); it!=shapes.end(); it++) {
                ShapeBase *shape = it->get();
                if(shape->hitTest(point, distanceThreshold)) {
                    selectedShapes.insert(shape);
                    found = true;
                }
            }
            if(bClearIfNoneFound && !found) clearSelection();
        }
        
        
        //--------------------------------------------------------------
        void ShapeManager::clearSelection() {
            selectedShapes.clear();
        }
        
        //--------------------------------------------------------------
        bool ShapeManager::hasSelection() const {
            return !selectedShapes.empty();
            
        }
        
        
#pragma mark file i/o
        void ShapeManager::save(string filename) {
            ofLogNotice() << "ShapeManager::save " << filename;
            ofxXmlSettings xml;
            xml.addTag("MSAFreehander");
            xml.pushTag("MSAFreehander");
            
            xml.addValue("version", getVersion());
            xml.addTag("shapes");
            xml.pushTag("shapes");
            
            for(list<ShapeBasePtr>::iterator it=shapes.begin(); it!=shapes.end(); it++) {
                ShapeBase *shape = it->get();
                int tagId = xml.addTag("shape");
                xml.pushTag("shape", tagId);
                xml.addValue("type", shape->getType());
                
                // write node transform
                xml.addTag("node");
                ofMatrix4x4 m44(shape->getGlobalTransformMatrix());
                for(int k=0; k<16; k++) {
                    xml.addAttribute("node", "i" + ofToString(k), m44.getPtr()[k], 0);
                }
                
                // write shape
                shape->writeToXml(xml);
                xml.popTag();   // shape
            }
            xml.popTag();   // shapes
            xml.popTag();   // MSAFreehander
            xml.save(filename);
        }
        
        
        //--------------------------------------------------------------
        void ShapeManager::load(string filename) {
            ofLogNotice() << "ShapeManager::load " << filename;
            ofxXmlSettings xml;
            if(xml.load(filename)) {
                if(xml.pushTag("MSAFreehander")) {
                    if(xml.pushTag("shapes")) {
                        int numShapes = xml.getNumTags("shape");
                        ofLogNotice() << "   loading " << numShapes << " shapes";
                        for(int s=0; s<numShapes; s++) {
                            xml.pushTag("shape", s);
                            
                            ShapeBase *shape = NULL;
                            string type = xml.getValue("type", "unknown");
                            if(type == "vector") shape = addVector();//new ShapeVector;
                            else if(type == "text") shape = addText();//new ShapeText;
                            
                            if(shape) {
                                // read node transform
                                ofMatrix4x4 m44;
                                for(int k=0; k<16; k++) {
                                    m44.getPtr()[k] = xml.getAttribute("node", "i" + ofToString(k), 0.0f, 0);
                                }
                                shape->setTransformMatrix(m44);
                                shape->readFromXml(xml);
                                
                                //                                addShape(shape);
                                
                            } else {
                                ofLogError() << "   Unknown shape type";
                            }
                            
                            
                            xml.popTag(); // shape
                        }
                        xml.popTag();   // shapes
                    } else {
                        ofLogError() << "   Could not open tag 'shapes'";
                    }
                    xml.popTag();   // MSAFreehander
                } else {
                    ofLogError() << "   Could not open tag 'MSAFreehander'";
                }
            } else {
                ofLogError() << "   Could not load file";
            }
            
            clearSelection();
        }
        
        
#pragma mark ofBaseDraws
        //--------------------------------------------------------------
        float ShapeManager::getHeight() {
            return boundingBox.width;
        }
        
        //--------------------------------------------------------------
        float ShapeManager::getWidth() {
            return boundingBox.height;
            
        }
        
        //--------------------------------------------------------------
        void ShapeManager::draw(float x, float y) {
            ofPushMatrix();
            ofTranslate(x, y);
            draw(false);
            ofPopMatrix();
        }
        
        
        //--------------------------------------------------------------
        void ShapeManager::draw(float x, float y, float w, float h) {
            ofPushMatrix();
            ofTranslate(x, y);
            ofScale(w / getWidth(), y / getHeight());
            draw(false);
            ofPopMatrix();
        }
        
        
        
    }
}