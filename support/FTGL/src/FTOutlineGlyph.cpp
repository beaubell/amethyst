#include    "FTOutlineGlyph.h"
#include    "FTVectoriser.h"


FTOutlineGlyph::FTOutlineGlyph( FT_GlyphSlot glyph )
:   FTGlyph( glyph)
{
    if( ft_glyph_format_outline != glyph->format)
    {
        err = 0x14; // Invalid_Outline
        return;
    }

    FTVectoriser vectoriser( glyph);

    size_t numContours = vectoriser.ContourCount();
    if ( ( numContours < 1) || ( vectoriser.PointCount() < 3))
    {
        return;
    }

    for( unsigned int c = 0; c < numContours; ++c)
    {
        const FTContour* contour = vectoriser.Contour(c);
        
        glBegin( GL_LINE_LOOP);
            for( unsigned int pointIndex = 0; pointIndex < contour->PointCount(); ++pointIndex)
            {
                FTPoint point = contour->Point(pointIndex);
                glVertex2f( point.X() / 64.0f, point.Y() / 64.0f);
            }
        glEnd();
    }

}


FTOutlineGlyph::~FTOutlineGlyph()
{

}


const FTPoint& FTOutlineGlyph::Render( const FTPoint& pen)
{
    glTranslatef( pen.X(), pen.Y(), 0.0f);

    //FIXME Render
    
    return advance;
}

FTPoint FTOutlineGlyph::Compose( const FTPoint& pen, TextHandle &hdl)
{
    //FIXME
    return advance;
}
