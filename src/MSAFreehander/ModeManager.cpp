#include "ModeManager.h"

namespace msa {
    namespace freehander {
        
        ModeManager *ModeManager::_instance = NULL;
        
        //--------------------------------------------------------------
        ModeManager::ModeManager() {
            setMode(kDraw);
        }
        
        //--------------------------------------------------------------
        ModeManager& ModeManager::instance() {
            if(!_instance) _instance = new ModeManager();
            return *_instance;
        }
        
        
        //--------------------------------------------------------------
        string ModeManager::getString() const {
            string s = "ModeManager:\n";
            s += "Mode: ";
            switch(_mode) {
                case kDraw: s+= "kDraw"; break;
                case kText: s+= "kText"; break;
                case kPan: s+= "kPan"; break;
                case kZoom: s+= "kZoom"; break;
                case kConnectLines: s+= "kConnectLines"; break;
                case kSelect: s+= "kSelect"; break;
                default:
                    s += "Unknown " + ofToString(_mode);
            }

            return s;
            
        }
        
        
        //--------------------------------------------------------------
        void ModeManager::setMode(Mode m) {
            if(m != _mode) {
                ofLogNotice() << "ModeManager::setMode " << m;
                _mode = m;
            }
        }
        
        //--------------------------------------------------------------
        bool ModeManager::toggleMode(Mode m) {
            if(isMode(m)) {
                setMode(kDraw);
                return false;
            } else {
                setMode(m);
                return true;
            }
        }

        //--------------------------------------------------------------
        ModeManager::Mode ModeManager::getMode() const {
            return _mode;
        }
        
        //--------------------------------------------------------------
        bool ModeManager::isMode(Mode m) const {
            return getMode() == m;
            
        }


    }
}

