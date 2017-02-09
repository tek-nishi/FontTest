//
// フォントテスト
// SOURCE: https://github.com/memononen/fontstash
//

#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <cinder/gl/gl.h>
#include <cinder/Camera.h>
#include "Defines.hpp"
#include "Path.hpp"
#include "Shader.hpp"
#include "Font.hpp"


namespace ngs {

// シェーダー作成
ci::gl::GlslProgRef createShader(const std::string& vtx_shader, const std::string& frag_shader) noexcept
{
  auto shader = readShader(vtx_shader, frag_shader);
  return ci::gl::GlslProg::create(shader.first, shader.second);
}


class FontTestApp
  : public ci::app::App
{
  Font font_ = { 1024, 1024, FONS_ZERO_BOTTOMLEFT };
  
  int fontNormal = FONS_INVALID;
  int fontJpn    = FONS_INVALID;

  ci::CameraOrtho camera_;

  
  ci::gl::GlslProgRef font_shader_;


  void setupCamera(const ci::vec2& size) noexcept
  {
    // 画面左下が原点
    camera_.setOrtho(0, size.x,
                     0, size.y,
                     -1.0f, 100.0f);
  }

  

public:
  FontTestApp() noexcept
  : font_shader_(createShader("font", "font"))
  {
    fontNormal = fonsAddFont(font_(), "sans", getAssetPath("DroidSerif-Regular.ttf").c_str());
    fontJpn    = fonsAddFont(font_(), "sans-jp", getAssetPath("DroidSansJapanese.ttf").c_str());

    setupCamera(ci::app::toPixels(ci::app::getWindowSize()));
  }

  ~FontTestApp() = default;


  void resize() noexcept override
  {
    setupCamera(ci::app::toPixels(ci::app::getWindowSize()));
  }

  
  void draw() noexcept override
  {
    ci::gl::clear(ci::Color(0.25, 0.25, 0.25));

    ci::gl::setMatrices(camera_);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    {
      ci::gl::ScopedGlslProg glsl(font_shader_);

      fonsClearState(font_());

      fonsSetSize(font_(), 120.0f);
      fonsSetFont(font_(), fontNormal);
      fonsSetColor(font_(), font_.color(0.25, 1, 1, 1));
      fonsDrawText(font_(), 100, 100, "Hoge Fuga Piyo", nullptr);

      fonsSetFont(font_(), fontJpn);
      fonsSetColor(font_(), font_.color(1, 1, 0.25, 1));
      fonsDrawText(font_(), 100, 200, "ほげ　ふが　ぴよ", nullptr);

      fonsSetSize(font_(), 20.0f);
      fonsSetColor(font_(), font_.color(1, 0.25, 1, 1));
      fonsDrawText(font_(), 50, 300, "シャーロック・ホームズシリーズは、今日で言うところの「読切連載」という形式で雑誌に掲載されました。", NULL);
    }

    {
      
      fonsClearState(font_());
      fonsSetSize(font_(), 120.0f);
      fonsSetFont(font_(), fontNormal);

      float bounds[4];
      float w = fonsTextBounds(font_(), 100, 100, "Hoge Fuga Piyo", nullptr, bounds);

      ci::gl::color(1, 1, 1);
      ci::gl::drawLine(ci::vec2(100, 100), ci::vec2(100 + w, 100));

      ci::gl::color(1, 1, 0);
      ci::gl::drawStrokedRect(ci::Rectf(bounds[0], bounds[1], bounds[2], bounds[3]));
    }
  }
};

}


CINDER_APP(ngs::FontTestApp, ci::app::RendererGl,
           [](ci::app::App::Settings* settings) noexcept
           {
             settings->setTitle(PREPRO_TO_STR(PRODUCT_NAME));
             // settings->setWindowSize(960, 640);
             // settings->setHighDensityDisplayEnabled(true);
           })
