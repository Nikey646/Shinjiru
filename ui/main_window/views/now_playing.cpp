#include "now_playing.h"
#include "ui_now_playing.h"

namespace Views {

NowPlaying::NowPlaying(QWidget *parent)
    : QWidget(parent), ui(new Ui::NowPlaying) {
  ui->setupUi(this);
}

NowPlaying::~NowPlaying() { delete ui; }

}  // namespace Views
