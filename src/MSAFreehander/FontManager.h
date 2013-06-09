#pragma once

#include "ofMain.h"

namespace msa {
    namespace freehander {
        class FontManager {
        public:
            void loadFont(string name, string path, int fontsize, bool _bAntiAliased=true, bool _bFullCharacterSet=false, bool makeContours=false, float simplifyAmt=0.3, int dpi=0);
            ofTrueTypeFont& getFont(string name);
            
            static FontManager& instance();
            
        private:
            FontManager();
            map<string, shared_ptr<ofTrueTypeFont> > fonts;
            static FontManager *_instance;
        };
    }
}