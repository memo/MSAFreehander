//
//  FontManager.cpp
//  MSAFreehander
//
//  Created by Memo Akten on 01/06/2013.
//
//

#include "FontManager.h"

namespace msa {
    namespace freehander {
        
        FontManager *FontManager::_instance = NULL;

        //--------------------------------------------------------------
        FontManager::FontManager() {
        }
        
        //--------------------------------------------------------------
        FontManager& FontManager::instance() {
            if(!_instance) _instance = new FontManager();
            return *_instance;
        }

        
        //--------------------------------------------------------------
        void FontManager::loadFont(string name, string path, int fontsize, bool _bAntiAliased, bool _bFullCharacterSet, bool makeContours, float simplifyAmt, int dpi) {
            fonts[name] = shared_ptr<ofTrueTypeFont>(new ofTrueTypeFont());
            fonts[name]->loadFont(path, fontsize, _bAntiAliased, _bFullCharacterSet, makeContours, simplifyAmt, dpi);
        }
        
        //--------------------------------------------------------------
        ofTrueTypeFont& FontManager::getFont(string name) {
            return *fonts[name];
        }
        
    }
}