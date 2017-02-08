#pragma once

//
// OSごとのパスの違いを吸収
//

#include <cinder/Utilities.h>


namespace ngs {

// アセットのパスを取得
//   ファイル読み込み時はこの関数でパスを取得する事
ci::fs::path getAssetPath(const std::string& path) noexcept
{
#if defined (DEBUG) && defined (CINDER_MAC)
  // OSX:Debug時はプロジェクトの場所からfileを読み込む
  ci::fs::path full_path(std::string(PREPRO_TO_STR(SRCROOT)) + "../assets/" + path);
  return full_path;
#else
  // TIPS:何気にci::app::getAssetPathがいくつかのpathを探してくれている
  auto full_path = ci::app::getAssetPath(path);
  return full_path;
#endif
}

// データ書き出しのパスを取得
//   ファイル書き出し時はこの関数でパスを取得する事
ci::fs::path getDocumentPath() noexcept
{
#if defined(CINDER_COCOA_TOUCH)
  // iOS:はアプリごとに用意された場所
  return ci::getDocumentsDirectory();
#elif defined (CINDER_MAC)
#if defined (DEBUG)
  // OSX:Debug時はプロジェクトの場所へ書き出す
  return ci::fs::path(PREPRO_TO_STR(SRCROOT));
#else
  // OSX:Release時はアプリコンテンツ内
  return ci::app::Platform::get()->getResourceDirectory();
#endif
#else
  // Widnows:実行ファイルと同じ場所
  return ci::app::getAppPath();
#endif
}

}
