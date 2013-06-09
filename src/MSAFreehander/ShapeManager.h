#pragma once

#include "ofMain.h"
#include "ModeManager.h"
#include "ShapeVector.h"
#include "ShapeText.h"
#include "BrushSettings.h"
#include <set>

namespace msa {
    namespace freehander {
        
        class ShapeManager : public ofBaseDraws {
        public:
            BrushSettings brushSettings;
            void reset();
            string getString() const;
            ofRectangle getBoundingBox() const;
            void draw(bool doDrawGrid, int gridSize = 0, ofRectangle drawRect = ofRectangle(0, 0, 0, 0));
            string getVersion();
            
#pragma mark Shape modifiers
            // ShapeVector
            void addVertex(ofVec2f p, bool bCreateNew);
            void incBrushThickness(float inc);
            void incBrushSmoothing(float inc);
            void setColor(int r, int g, int b);
            // ShapeText
            void addKey(int key);
            
            void moveBy(ofVec2f offset);
            
            
#pragma mark add/remove/get shapes
            void deleteSelectedShapes();
            void deleteLastElement();
            
            ShapeVector *addVector(ofVec2f pos = ofVec2f());
            ShapeText *addText(ofVec2f pos = ofVec2f());
            ShapeBase *addShape(ShapeBase *s);
            void cleanShapes();
            void duplicate();

#pragma mark Selection
            void selectShapes(ofVec2f point, float distanceThreshold, bool bClearIfNoneFound);
            void clearSelection();
            bool hasSelection() const;
            
#pragma mark file i/o
            void save(string filename);
            void load(string filename);

            
#pragma mark ofBaseDraws
            virtual float getHeight();
            virtual float getWidth();
            virtual void draw(float x, float y);
            virtual void draw(float x, float y, float w, float h);
            
        protected:
            bool bDrawProcessed = true;
            ofRectangle boundingBox;
            
            struct {
                int totalVerts;
                int drawnVerts;
                int drawnShapes;
            } num;
            
            list<ShapeBasePtr> shapes;
            set<ShapeBase*> selectedShapes;
        };
        
    }
}