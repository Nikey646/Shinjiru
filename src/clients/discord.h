#ifndef DISCORD_H
#define DISCORD_H

#include "../utilities/singleton.h"

#include <discord_rpc.h>

class Discord : public Singleton<Discord> {
  Q_OBJECT

 public:
  Discord();
  ~Discord();

  void updatePresence(const QString &title, const int episode);
  void clearPresence();
};

#endif  // DISCORD_H
