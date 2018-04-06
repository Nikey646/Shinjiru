#ifndef DISCORD_H
#define DISCORD_H

#include "../settings.h"
#include "../utilities/singleton.h"

#include <discord_rpc.h>

class Discord : public Singleton<Discord> {
  Q_OBJECT

 public:
  Discord();
  ~Discord();

  void updatePresence(const QString &title, const int episode);
  void clearPresence();

 private:
  Settings s;
  bool initialized{false};
};

#endif  // DISCORD_H
