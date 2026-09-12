
//{{BLOCK(background)

//======================================================================
//
//	background, 240x160@8, 
//	+ palette 16 entries, not compressed
//	+ 182 tiles (t|f reduced) not compressed
//	+ regular map (flat), not compressed, 30x20 
//	Total size: 32 + 11648 + 1200 = 12880
//
//	Time-stamp: 2026-09-03, 02:25:23
//	Exported by Cearn's GBA Image Transmogrifier, v0.9.2
//	( http://www.coranac.com/projects/#grit )
//
//======================================================================

#ifndef GRIT_BACKGROUND_H
#define GRIT_BACKGROUND_H

#define backgroundTilesLen 11648
extern const unsigned int backgroundTiles[2912];

#define backgroundMapLen 1200
extern const unsigned short backgroundMap[600];

#define backgroundPalLen 32
extern const unsigned short backgroundPal[16];

#endif // GRIT_BACKGROUND_H

//}}BLOCK(background)
