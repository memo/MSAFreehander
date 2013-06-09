#pragma once

#include "ofMain.h"

namespace msa {
    namespace freehander {
        
        class ModeManager {
        public:
            
            enum Mode {
                kDraw,
                kText,
                kPan,
                kZoom,
                kConnectLines,
                kSelect,
            };
            
            static ModeManager& instance();
            
            string getString() const;

            void setMode(Mode m);
            bool toggleMode(Mode m);
            Mode getMode() const;
            bool isMode(Mode m) const;
            
        private:
            ModeManager();
            
            Mode _mode;

            static ModeManager *_instance;
        };
    }
}