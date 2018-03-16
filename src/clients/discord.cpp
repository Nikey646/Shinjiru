#include "discord.h"

#include <QDebug>

#include <sstream>

Discord::Discord() {
#ifndef QT_DEBUG
  DiscordEventHandlers handlers;
  memset(&handlers, 0, sizeof(handlers));

  handlers.ready = []() { qDebug() << "Discord is ready!"; };

  handlers.errored = [](int errc, const char *msg) {
    qWarning() << "Error " << errc << ": " << msg;
    ;
  };

  handlers.disconnected = [](int errc, const char *msg) {
    qWarning() << "Disconnected " << errc << ": " << msg;
  };

  Discord_Initialize("378322625321107457", &handlers, 0, NULL);
#endif
}

Discord::~Discord() {
#ifndef QT_DEBUG
  Discord_Shutdown();
#endif
}

void Discord::updatePresence(const QString &title, const int episode) {
#ifndef QT_DEBUG
  DiscordRichPresence discordPresence;
  memset(&discordPresence, 0, sizeof(discordPresence));

  std::stringstream oss;
  oss << "Episode " << episode;

  discordPresence.state = qPrintable(title);
  discordPresence.details = oss.str().c_str();
  discordPresence.largeImageKey = "shinjiru";
  discordPresence.largeImageText = "Shinjiru";
  discordPresence.instance = 0;
  discordPresence.matchSecret = "";

  Discord_UpdatePresence(&discordPresence);
#endif
}

void Discord::clearPresence() {
#ifndef QT_DEBUG
  Discord_ClearPresence();
#endif
}
