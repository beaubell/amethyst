#include    "FTGlyphContainer.h"
#include    "FTGlyph.h"
#include    "FTFace.h"
#include    "FTCharmap.h"

namespace ftgl {

FTGlyphContainer::FTGlyphContainer( FTFace* f)
:   face(f),
    err(0)
{
    glyphs.push_back( NULL);
    charMap = new FTCharmap( face);
}


FTGlyphContainer::~FTGlyphContainer()
{
    GlyphVector::iterator glyphIterator;
    for( glyphIterator = glyphs.begin(); glyphIterator != glyphs.end(); ++glyphIterator)
    {
        delete *glyphIterator;
    }
    
    glyphs.clear();
    delete charMap;
}


bool FTGlyphContainer::CharMap( FT_Encoding encoding)
{
    bool result = charMap->CharMap( encoding);
    err = charMap->Error();
    return result;
}


unsigned int FTGlyphContainer::FontIndex( const unsigned int characterCode) const
{
    return charMap->FontIndex( characterCode);
}


void FTGlyphContainer::Add( FTGlyph* tempGlyph, const unsigned int characterCode)
{
    charMap->InsertIndex( characterCode, glyphs.size());
    glyphs.push_back( tempGlyph);
}


const FTGlyph* FTGlyphContainer::Glyph( const unsigned int characterCode) const
{
    signed int index = charMap->GlyphListIndex( characterCode);
    return glyphs[index];
}


FTBBox FTGlyphContainer::BBox( const unsigned int characterCode) const
{
    return glyphs[charMap->GlyphListIndex( characterCode)]->BBox();
}


float FTGlyphContainer::Advance( const unsigned int characterCode, const unsigned int nextCharacterCode)
{
    unsigned int left = charMap->FontIndex( characterCode);
    unsigned int right = charMap->FontIndex( nextCharacterCode);

    float width = face->KernAdvance( left, right).X();
    width += glyphs[charMap->GlyphListIndex( characterCode)]->Advance().X();
    
    return width;
}


FTPoint FTGlyphContainer::Render( const unsigned int characterCode, const unsigned int nextCharacterCode, FTPoint penPosition)
{
    FTPoint kernAdvance, advance;
    
    unsigned int left = charMap->FontIndex( characterCode);
    unsigned int right = charMap->FontIndex( nextCharacterCode);

    kernAdvance = face->KernAdvance( left, right);
        
    if( !face->Error())
    {
        advance = glyphs[charMap->GlyphListIndex( characterCode)]->Render( penPosition);
    }
    
    kernAdvance += advance;
    return kernAdvance;
}

FTPoint FTGlyphContainer::Compose( const unsigned int characterCode, const unsigned int nextCharacterCode, FTPoint penPosition, TextHandle &hdl)
{
    FTPoint kernAdvance, advance;
    
    unsigned int left = charMap->FontIndex( characterCode);
    unsigned int right = charMap->FontIndex( nextCharacterCode);

    kernAdvance = face->KernAdvance( left, right);
        
    if( !face->Error())
    {
        advance = glyphs[charMap->GlyphListIndex( characterCode)]->Compose( penPosition, hdl);
    }
    
    kernAdvance += advance;
    return kernAdvance;
}

} //namespace ftgl
