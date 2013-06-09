MSAFreehander is a very minimal and crude freehand drawing app I wrote for my workshop at EYEO 2013 entitled "The black art of manipulating numbers. Essential applied maths for computational artists." A workshop covering fundamentals and applications of trigonometry, vector maths, matrices etc. 
http://eyeofestival.com/speaker/memo-akten/

Instead of preparing hundreds of slides, I wanted to just sit down and draw, as if I had an overhead projector, or a whiteboard. In essence MSAFreehander is an overhead projector simulator application.

Example output: 
http://farm9.staticflickr.com/8545/8966343618_15702447e0_o.png


Some key features I wanted were:
- infinite canvas, with simple and quick to activate pan and zoom controls
- simple access to predefined colours, brush size etc.
- save / load as XML
- interactive navigator map view shows whole canvas, which you can zoom and pan on as well
- select, move, edit already drawn shapes
- type in text



//--------------------------------------------------------------
INSTRUCTIONS (for using the app)
Launch the app, and draw with your mouse! (plugging in a wacom makes it much simpler and easier to use)

You can be in one of these PERMANENT modes:
DRAW: click and drag on the canvas to draw
(tap 'd' to switch to DRAW)

SELECT: click on one (or more) shapes to select. once selected you can move, delete and edit
(tap SPACEBAR to toggle between SELECT and DRAW)

TEXT: click on the canvas to insert text, start typing. ENTER to finish.
(tap 't' to toggle between TEXT and DRAW)


You can also be in one of these TEMPORARY modes:
(these are only active while the relevant key is held down)
ZOOM: click and drag on the canvas to zoom in/out
(hold down 'z' to activate ZOOM, release to switch back to DRAW)

PAN: click and drag on the canvas to pan
(hold down 'x' to activate PAN, release to switch back to DRAW)

CONNECT: instead of creating a new shape, draw line between the last mouse click and new mouse click (useful for drawing polygons)
(hold down 'TAB' to activate CONNECT, release to switch back to normal DRAW)


OTHER KEYS:
Display:
f: toggle fullscreen
g: toggle grid
e: zoom extents
,.: zoom in/out
ARROWS: pan


I/O
s: save canvas as XML
l: load canvas as XML

Modifying:
SHIFT + N: start new canvas
BACKSPACE: erase last point (on selected shapes) or letter (on selected texts)
SHIFT + BACKSPACE: erase selected shape(s)
0-6: colours (black, red, green, blue, yellow, purple, cyan)
[]: change brush thickness of selected shapes OR for next drawing (if no shapes are selected)
{}: change brush smoothing of selected shapes OR for next drawing (if no shapes are selected)



//--------------------------------------------------------------
INSTRUCTIONS (for compiling the source)
The source is written ontop of openFrameworks (0.7.4)
https://github.com/openframeworks/openFrameworks

and uses my ofxMSAInteractiveObject add-on 
https://github.com/memo/ofxMSAInteractiveObject



//--------------------------------------------------------------
TODOS
(in order of priority)
- optimise drawing (use VBOs)
- optimise drawing of navigation map (cache all unmodified shapes to fbo?)
- save / load as SVG
- render to PDF (quite buggy right now)
- load (bitmap) images
- undo
- put all settings in a settings XML including colours and keyboard shortcuts
- multiple pages (currently can be faked by saving / loading canvases)
- have a 'presenter' view which loads text file showing the topics you want to cover, with a timer (similar to Keynote.app)
- use extra wacom data (pressure, angle etc).