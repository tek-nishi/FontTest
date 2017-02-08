//
// フォントテスト
// SOURCE: https://github.com/memononen/fontstash
//

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <cinder/gl/gl.h>

#include "Defines.hpp"
#include "Path.hpp"
#include "Shader.hpp"
#include "FontRender.hpp"


namespace ngs {

// シェーダー作成
ci::gl::GlslProgRef createShader(const std::string& vtx_shader, const std::string& frag_shader) noexcept
{
  auto shader = readShader(vtx_shader, frag_shader);
  return ci::gl::GlslProg::create(shader.first, shader.second);
}


class FontTestApp : public ci::app::App
{
  FONScontext* fs = NULL;

  int fontNormal = FONS_INVALID;
  int fontJpn    = FONS_INVALID;

  ci::gl::GlslProgRef font_shader_;


public:
  FontTestApp() noexcept
  {
    fs = fontInit(512, 512, FONS_ZERO_TOPLEFT);

    fontNormal = fonsAddFont(fs, "sans", getAssetPath("DroidSerif-Regular.ttf").string().c_str());
    fontJpn    = fonsAddFont(fs, "sans-jp", getAssetPath("DroidSansJapanese.ttf").string().c_str());

    font_shader_ = createShader("font", "font");
  }

  ~FontTestApp() noexcept
  {
    fontDelete(fs);
  }


  void draw() noexcept override
  {
    ci::gl::clear(ci::Color(0.5, 0.5, 0.5));

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    ci::gl::ScopedGlslProg glsl(font_shader_);

    fonsClearState(fs);

    fonsSetSize(fs, 64.0f);
    fonsSetFont(fs, fontNormal);
    fonsSetColor(fs, fontRGBA(64, 255, 255, 255));
    fonsDrawText(fs, 100, 100, "Hoge Fuga Piyo", NULL);

    fonsSetFont(fs, fontJpn);
    fonsSetColor(fs, fontRGBA(255, 255, 64, 255));
    fonsDrawText(fs, 100, 200, "ほげ　ふが　ぴよ", NULL);

    fonsSetSize(fs, 16.0f);
    fonsSetColor(fs, fontRGBA(255, 64, 255, 255));
    fonsDrawText(fs, 50, 300, "シャーロック・ホームズシリーズは、今日で言うところの「読切連載」という形式で雑誌に掲載されました。", NULL);
  }
};

}


CINDER_APP(ngs::FontTestApp, ci::app::RendererGl,
           [](ci::app::App::Settings* settings) noexcept
           {
             settings->setTitle(PREPRO_TO_STR(PRODUCT_NAME));
           })
