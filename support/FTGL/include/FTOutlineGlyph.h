#ifndef		__FTOutlineGlyph__
#define		__FTOutlineGlyph__

#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H

#include "FTGL.h"
#include "FTGlyph.h"

namespace ftgl {

class FTVectoriser;


/**
 * FTOutlineGlyph is a specialisation of FTGlyph for creating outlines.
 * 
 * @see FTGlyphContainer
 * @see FTVectoriser
 *
 */
class FTGL_EXPORT FTOutlineGlyph : public FTGlyph
{
    public:
        /**
         * Constructor. Sets the Error to Invalid_Outline if the glyphs isn't an outline.
         *
         * @param glyph The Freetype glyph to be processed
         * @param useDisplayList Enable or disable the use of Display Lists for this glyph
         *                       <code>true</code> turns ON display lists.
         *                       <code>false</code> turns OFF display lists.
         */
        FTOutlineGlyph( FT_GlyphSlot glyph );

        /**
         * Destructor
         */
        virtual ~FTOutlineGlyph();

        /**
         * Renders this glyph at the current pen position.
         *
         * @param pen	The current pen position.
         * @return		The advance distance for this glyph.
         */
        virtual const FTPoint& Render( const FTPoint& pen);
	virtual       FTPoint  Compose( const FTPoint& pen, TextHandle &hdl);
        
    private:		
	
};

} //namespace ftgl

#endif	//	__FTOutlineGlyph__

