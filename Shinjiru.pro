QT += core gui widgets network networkauth

TARGET = Shinjiru
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

include(lib/robot.pri)
include(lib/anitomy.pri)
include(lib/discord-rpc.pri)

SOURCES += \
  src/clients/anilist.cpp \
  src/clients/discord.cpp \
  src/clients/graphql_query.cpp \
  src/clients/masato.cpp \
  src/clients/nekomimi.cpp \
  src/detection/media_store.cpp \
  src/detection/window_enumerator.cpp \
  src/main.cpp \
  src/models/media.cpp \
  src/models/media_list.cpp \
  src/models/rss_item.cpp \
  src/models/user.cpp \
  src/settings.cpp \
  src/utilities/file_downloader.cpp \
  ui/anime_panel.cpp \
  ui/main_window.cpp \
  ui/main_window/components/media_table_model.cpp \
  ui/main_window/components/media_table_proxy_model.cpp \
  ui/main_window/components/progress_item_delegate.cpp \
  ui/main_window/components/rss_table_model.cpp \
  ui/main_window/components/score_item_delegate.cpp \
  ui/main_window/components/status_item_delegate.cpp \
  ui/main_window/views/airing.cpp \
  ui/main_window/views/anime_list.cpp \
  ui/main_window/views/anime_table.cpp \
  ui/main_window/views/now_playing.cpp \
  ui/main_window/views/torrents.cpp \
  ui/settings_dialog.cpp \
  ui/settings_dialog/views/application.cpp \
  ui/settings_dialog/views/recognition.cpp \
  ui/settings_dialog/views/torrent_settings.cpp

HEADERS += \
  src/clients/anilist.h \
  src/clients/discord.h \
  src/clients/graphql_query.h \
  src/clients/masato.h \
  src/clients/nekomimi.h \
  src/detection/media_store.h \
  src/detection/window_enumerator.h \
  src/models/media.h \
  src/models/media_list.h \
  src/models/rss_item.h \
  src/models/user.h \
  src/settings.h \
  src/utilities/file_downloader.h \
  src/utilities/singleton.h \
  ui/anime_panel.h \
  ui/main_window.h \
  ui/main_window/components/media_table_model.h \
  ui/main_window/components/media_table_proxy_model.h \
  ui/main_window/components/progress_item_delegate.h \
  ui/main_window/components/rss_table_model.h \
  ui/main_window/components/score_item_delegate.h \
  ui/main_window/components/status_item_delegate.h \
  ui/main_window/views/airing.h \
  ui/main_window/views/anime_list.h \
  ui/main_window/views/anime_table.h \
  ui/main_window/views/now_playing.h \
  ui/main_window/views/torrents.h \
  ui/settings_dialog.h \
  ui/settings_dialog/views/application.h \
  ui/settings_dialog/views/recognition.h \
  ui/settings_dialog/views/torrent_settings.h

FORMS += \
  ui/anime_panel.ui \
  ui/main_window.ui \
  ui/main_window/views/airing.ui \
  ui/main_window/views/animelist.ui \
  ui/main_window/views/now_playing.ui \
  ui/main_window/views/torrents.ui \
  ui/settings_dialog.ui \
  ui/settings_dialog/views/application.ui \
  ui/settings_dialog/views/recognition.ui \
  ui/settings_dialog/views/torrent_settings.ui \

INCLUDEPATH += \
  lib

RESOURCES += \
  res.qrc

TRANSLATIONS = \
  lang/shinjiru_en.ts
