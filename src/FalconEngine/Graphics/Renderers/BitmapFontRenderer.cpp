#include <FalconEngine/Graphics/Renderers/BitmapFontRenderer.h>

#include <algorithm>
#include <numeric>

#include <boost/algorithm/string.hpp>
#include <glm/gtc/matrix_transform.inl>
#include <glm/gtc/type_ptr.hpp>

#include <FalconEngine/Content/AssetManager.h>
#include <FalconEngine/Graphics/Effects/BitmapFontEffect.h>
#include <FalconEngine/Graphics/Renderers/BitmapLine.h>
#include <FalconEngine/Graphics/Renderers/Renderer.h>
#include <FalconEngine/Graphics/Renderers/VisualQuads.h>
#include <FalconEngine/Graphics/Renderers/Resources/Texture2dArray.h>

using namespace std;
using namespace FalconEngine;

namespace FalconEngine
{

/************************************************************************/
/* Constructors and Destructor                                          */
/************************************************************************/
BitmapFontRenderer::BitmapFontRenderer() :
    mDynamicTextBuffer(nullptr),
    mDynamicTextQuads(nullptr),
    mStaticTextBuffer(nullptr),
    mStaticTextQuads(nullptr),
    mTextEffect()
{
}

BitmapFontRenderer::~BitmapFontRenderer()
{
}

/************************************************************************/
/* Public Members                                                       */
/************************************************************************/
void
BitmapFontRenderer::Initialize(int width, int height)
{
    auto assetManager = AssetManager::GetInstance();

    // Setup text effect.
    {
        auto font = assetManager->LoadFont("Content/Fonts/LuciadaConsoleDistanceField.bin");

        // Prepare text effect.
        mTextEffect = make_shared<BitmapFontEffect>(&mTextHandedness);
        mTextEffectInstance = mTextEffect->CreateInstance(font, width, height);

    }

    auto textVertexFormat = mTextEffect->CreateVertexFormat();

    // Setup static text rendering.
    {
        // TODO(Wuxiang): Should I change the size of the buffer?
        mStaticTextBuffer = make_shared<VertexBuffer>(1024, sizeof(BitmapFontVertex), BufferUsage::Dynamic);

        auto textVertexGroupDynamic = make_shared<VertexGroup>();
        textVertexGroupDynamic->SetVertexBuffer(0, mStaticTextBuffer, 0, textVertexFormat->GetVertexAttributeStride());
        mStaticTextQuads = make_shared<VisualQuads>(textVertexFormat, textVertexGroupDynamic);

        mStaticTextItem = BitmapFontRenderItem();
    }

    // Setup dynamic text rendering.
    {
        // TODO(Wuxiang): Should I change the size of the buffer?
        mDynamicTextBuffer = make_shared<VertexBuffer>(1024, sizeof(BitmapFontVertex), BufferUsage::Stream);

        auto dynamicTextVertexGroup = make_shared<VertexGroup>();
        dynamicTextVertexGroup->SetVertexBuffer(0, mDynamicTextBuffer, 0, textVertexFormat->GetVertexAttributeStride());
        mDynamicTextQuads = make_shared<VisualQuads>(textVertexFormat, dynamicTextVertexGroup);
        //mDynamicTextQuads->SetVertexNum()

        mDynamicTextItem = BitmapFontRenderItem();
    }
}

void
BitmapFontRenderer::BatchTextDynamic(
    const BitmapFont *font,
    float             fontSize,
    std::string textString,
    Vector2f    textPosition,
    float       textLineWidth,
    Color       textColor)
{
    auto text = BitmapText(fontSize, textString, textPosition, textLineWidth);
    PrepareText(mDynamicTextItem, font, &text, textColor);
}

void
BitmapFontRenderer::BatchTextStatic(const BitmapFont *font, float fontSize, std::string textString, Vector2f textPosition, float textLineWidth, Color textColor)
{
    auto text = BitmapText(fontSize, textString, textPosition, textLineWidth);
    PrepareText(mStaticTextItem, font, &text, textColor);
}

// @summary Construct all the necessary lexical separation of given text into
// lines of words.
//
// @return The glyph number inside the text lines.
int
CreateTextLines(
    _IN_  const BitmapFont   *font,
    _IN_  const BitmapText   *text,
    _OUT_ vector<BitmapLine>& lines)
{
    using namespace boost;
    static auto lineStrings = vector<string>();
    lineStrings.clear();
    split(lineStrings, text->mTextString, is_any_of("\n"));

    // Bounds is formatted as [x, y, width, height]
    const auto lineWidth = text->mTextBounds[2];

    int glyphCount = 0;
    auto lineCurrent = BitmapLine(lineWidth);
    for (int lineIndex = 0; lineIndex < lineStrings.size(); ++lineIndex)
    {
        for (const wchar_t& c : lineStrings[lineIndex])
        {
            // NOTE(Wuxiang): When processing English, a word is defined as space
            // separated letters. But this concept would not work
            // on Chinese, when You still need to crop the word when the maximum line
            // width is exceeded. The arbitrariness exists here for English or
            // Chinese. If you don't have the pre-knowledge you won't be able to
            // decide which way is correct. My decision here is to leave this to
            // higher level code to preprocess this kind of information. This function
            // only deals new line characters.

            uint32 glyphCodepoint = c;
            auto   glyphIndex = font->mGlyphIndexTable.at(glyphCodepoint);
            auto&  glyph = font->mGlyphTable.at(glyphIndex);
            ++glyphCount;

            double fontSizeScale = text->mFontSize / font->mSizePt;
            bool success = lineCurrent.PushGlyph(glyph, fontSizeScale);
            if (!success)
            {
                // Have to start a new line for this glyph
                lines.push_back(lineCurrent);
                lineCurrent = BitmapLine(lineWidth);
                lineCurrent.PushGlyph(glyph, fontSizeScale);
            }
        }

        // NOTE(Wuxiang): There will be at least one line even the string is empty.
        lines.push_back(lineCurrent);
    }

    return glyphCount;
}

template<typename T>
inline void FillData(T *data, size_t& dataIndex, T value)
{
    data[dataIndex] = value;
    ++dataIndex;
}

void
FillFontAttribute(float             *textData,
                  size_t&            textDataIndex,
                  const BitmapGlyph *textGlyph,
                  Vector4f           textColor,
                  double fontSizeScale)
{
    // Font color
    FillData<float>(textData, textDataIndex, textColor.x);
    FillData<float>(textData, textDataIndex, textColor.y);
    FillData<float>(textData, textDataIndex, textColor.z);
    FillData<float>(textData, textDataIndex, textColor.w);

    // NOTE(Wuxiang): 1.32 is value the when imported font size is 33. 1.32 is
    // used as origin for scaling.

    // Font width
    FillData<float>(textData, textDataIndex, 0.50 * (1 + 0.15 * (fontSizeScale - 1.32)));

    // Font edge
    FillData<float>(textData, textDataIndex, 0.03 / (1 + 1.05 * (fontSizeScale - 1.32)));

    // Font page
    FillData<float>(textData, textDataIndex, textGlyph->mPage);
}

void
FillGlyphAttribute(float             *textData,
                   size_t&            textDataIndex,
                   const BitmapGlyph *textGlyph,
                   Vector4f           textColor,
                   float              textGlyphX,
                   float              textGlyphY,
                   const BitmapFont *font,
                   double            fontSizeScale)
{
    // NOTE(Wuxiang): Since x1, y1 represents left-bottom coordinate, we need to
    // process base and yoffset differently. Notably, x1 is amended to center the
    // glyph for different width, 'x + advance / 2 - width / 2' is the x1
    // considering no x offset. I don't think this cause the glyph to draw as
    // monospaced, since the advance would be different for different character
    // set, which means English would have different advance than Chinese.
    // However, the influence of width indeed is compensated using this method in
    // spacing of the glyph.
    double x1 = textGlyphX + (textGlyph->mAdvance / 2 - textGlyph->mWidth / 2 + textGlyph->mOffsetX) * fontSizeScale;
    double y2 = textGlyphY + (font->mLineBase - textGlyph->mOffsetY) * fontSizeScale;
    double y1 = y2 - textGlyph->mHeight * fontSizeScale;
    double x2 = x1 + textGlyph->mWidth * fontSizeScale;

    FillData<float>(textData, textDataIndex, x1);
    FillData<float>(textData, textDataIndex, y1);
    FillData<float>(textData, textDataIndex, textGlyph->mS1);
    FillData<float>(textData, textDataIndex, textGlyph->mT1);
    FillFontAttribute(textData, textDataIndex, textGlyph, textColor, fontSizeScale);

    FillData<float>(textData, textDataIndex, x2);
    FillData<float>(textData, textDataIndex, y2);
    FillData<float>(textData, textDataIndex, textGlyph->mS2);
    FillData<float>(textData, textDataIndex, textGlyph->mT2);
    FillFontAttribute(textData, textDataIndex, textGlyph, textColor, fontSizeScale);

    FillData<float>(textData, textDataIndex, x1);
    FillData<float>(textData, textDataIndex, y2);
    FillData<float>(textData, textDataIndex, textGlyph->mS1);
    FillData<float>(textData, textDataIndex, textGlyph->mT2);
    FillFontAttribute(textData, textDataIndex, textGlyph, textColor, fontSizeScale);

    FillData<float>(textData, textDataIndex, x2);
    FillData<float>(textData, textDataIndex, y1);
    FillData<float>(textData, textDataIndex, textGlyph->mS2);
    FillData<float>(textData, textDataIndex, textGlyph->mT1);
    FillFontAttribute(textData, textDataIndex, textGlyph, textColor, fontSizeScale);

    FillData<float>(textData, textDataIndex, x2);
    FillData<float>(textData, textDataIndex, y2);
    FillData<float>(textData, textDataIndex, textGlyph->mS2);
    FillData<float>(textData, textDataIndex, textGlyph->mT2);
    FillFontAttribute(textData, textDataIndex, textGlyph, textColor, fontSizeScale);

    FillData<float>(textData, textDataIndex, x1);
    FillData<float>(textData, textDataIndex, y1);
    FillData<float>(textData, textDataIndex, textGlyph->mS1);
    FillData<float>(textData, textDataIndex, textGlyph->mT1);
    FillFontAttribute(textData, textDataIndex, textGlyph, textColor, fontSizeScale);
}

// @summary Fill the vertex buffer with the text line information.
void
FillTextLines(
    _IN_ const BitmapFont   *font,
    _IN_ float               fontSize,
    _IN_ Vector2f                  textPosition,
    _IN_ Vector4f                  textColor,
    _IN_ const vector<BitmapLine>& textLines,
    _IN_ size_t&                   textDataIndex,
    _OUT_ float                   *textData
)
{
    float x = textPosition.x;
    float y = textPosition.y;

    double fontSizeScale = fontSize / font->mSizePt;

    for(auto& line : textLines)
    {
        for (auto& glyph : line.mLineGlyphs)
        {
            FillGlyphAttribute(textData, textDataIndex, &glyph, textColor, x, y, font, fontSizeScale);

            x += glyph.mAdvance * fontSizeScale;
        }

        x = textPosition.x;
        y -= font->mLineHeight * fontSizeScale;
    }
}

void
BitmapFontRenderer::PrepareText(
    BitmapFontRenderItem& item,
    const BitmapFont *font,
    const BitmapText *text,
    Color             textColor)
{
    static auto textLines = vector<BitmapLine>();
    textLines.clear();

    // Construct lines with glyph information.
    int textGlyphCount = CreateTextLines(font, text, textLines);

    // Fill the vertex attribute into the buffer
    FillTextLines(font, text->mFontSize, Vector2f(text->mTextBounds.x, text->mTextBounds.y), Vector4f(textColor), textLines, item.mTextBufferDataIndex, reinterpret_cast<float *>(item.mTextBuffer->mData));
}

void BitmapFontRenderer::RenderBegin()
{
}

void BitmapFontRenderer::Render(Renderer *renderer)
{
    renderer->Draw(&mStaticTextQuads, mTextEffectInstance.get());
    renderer->Draw(&mDynamicTextQuads, mTextEffectInstance.get());

    //// NOTE(Wuxiang): Use orphaning strategy
    //// TODO(Wuxiang): Wrong. Use glMapBuffer to access asynchronously
    //glBindBuffer(GL_ARRAY_BUFFER, renderGroupBuffer);
    //glBufferData(GL_ARRAY_BUFFER, renderGroupAttributes.size() * sizeof(float), nullptr, GL_STREAM_DRAW);
    //auto *va = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

    //// TODO(Move this to data creation?)
    //va copy &renderGroupAttributes[0];

    //glDrawArrays(GL_TRIANGLES, 0, renderGroup.mTextVertexCount);
}

void BitmapFontRenderer::RenderEnd()
{
}

}