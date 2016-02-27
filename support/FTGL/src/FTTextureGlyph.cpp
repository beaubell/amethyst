#include    "FTTextureGlyph.h"

GLint FTTextureGlyph::activeTextureID = 0;
 
FTTextureGlyph::FTTextureGlyph( FT_GlyphSlot glyph, int id, int xOffset, int yOffset, GLsizei width, GLsizei height)
:   FTGlyph( glyph),
    destWidth(0),
    destHeight(0),
    glTextureID(id)
{
    err = FT_Render_Glyph( glyph, FT_RENDER_MODE_NORMAL);
    if( err || glyph->format != ft_glyph_format_bitmap)
    {
        return;
    }

    FT_Bitmap      bitmap = glyph->bitmap;

    destWidth  = bitmap.width;
    destHeight = bitmap.rows;
    
    if( destWidth && destHeight)
    {
        //DEPRECATED glPushClientAttrib( GL_CLIENT_PIXEL_STORE_BIT);
        glPixelStorei( GL_UNPACK_LSB_FIRST, GL_FALSE);
        glPixelStorei( GL_UNPACK_ROW_LENGTH, 0);
        glPixelStorei( GL_UNPACK_ALIGNMENT, 1);

        glBindTexture( GL_TEXTURE_2D, glTextureID);
        glTexSubImage2D( GL_TEXTURE_2D, 0, xOffset, yOffset, destWidth, destHeight, GL_ALPHA, GL_UNSIGNED_BYTE, bitmap.buffer);

        //DEPRECATED glPopClientAttrib();
    }


//      0    
//      +----+
//      |    |
//      |    |
//      |    |
//      +----+
//           1
    
    uv[0].X( static_cast<float>(xOffset) / static_cast<float>(width));
    uv[0].Y( static_cast<float>(yOffset) / static_cast<float>(height));
    uv[1].X( static_cast<float>( xOffset + destWidth) / static_cast<float>(width));
    uv[1].Y( static_cast<float>( yOffset + destHeight) / static_cast<float>(height));
    
    pos.X( glyph->bitmap_left);
    pos.Y( glyph->bitmap_top);
}


FTTextureGlyph::~FTTextureGlyph()
{}


const FTPoint& FTTextureGlyph::Render( const FTPoint& pen)
{
    if( activeTextureID != glTextureID)
    {
        glBindTexture( GL_TEXTURE_2D, (GLuint)glTextureID);
        activeTextureID = glTextureID;
    }
    /* DEPRECATED
    glTranslatef( pen.X(),  pen.Y(), 0.0f);

    glBegin( GL_QUADS);
        glTexCoord2f( uv[0].X(), uv[0].Y());
        glVertex2f( pos.X(), pos.Y());

        glTexCoord2f( uv[0].X(), uv[1].Y());
        glVertex2f( pos.X(), pos.Y() - destHeight);

        glTexCoord2f( uv[1].X(), uv[1].Y());
        glVertex2f( destWidth + pos.X(), pos.Y() - destHeight);
        
        glTexCoord2f( uv[1].X(), uv[0].Y());
        glVertex2f( destWidth + pos.X(), pos.Y());
    glEnd();
    */
    return advance;
}

FTPoint FTTextureGlyph::Compose( const FTPoint& pen, TextHandle &hdl)
{
    glm::vec2 texcoord1 = glm::vec2(uv[0].X(), uv[0].Y());
    glm::vec4 vertex1   = glm::vec4(pen.X(), pen.Y(), 0.0f, 1.0f);

    glm::vec2 texcoord2 = glm::vec2(uv[0].X(), uv[1].Y());
    glm::vec4 vertex2   = glm::vec4(pen.X(), pen.Y() + destHeight, 0.0f, 1.0f);

    glm::vec2 texcoord3 = glm::vec2( uv[1].X(), uv[1].Y());
    glm::vec4 vertex3   = glm::vec4(pen.X()+destWidth, pen.Y() + destHeight, 0.0f, 1.0f);
    
    glm::vec2 texcoord4 = glm::vec2(uv[1].X(), uv[0].Y());
    glm::vec4 vertex4   = glm::vec4(pen.X()+destWidth, pen.Y(), 0.0f, 1.0f);

    hdl.addTriangle(glTextureID, vertex1, texcoord1, vertex2, texcoord2, vertex4, texcoord4);
    hdl.addTriangle(glTextureID, vertex2, texcoord2, vertex3, texcoord3, vertex4, texcoord4);
    
    return advance + pen;
}
