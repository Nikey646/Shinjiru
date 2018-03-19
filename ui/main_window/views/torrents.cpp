#include "torrents.h"
#include "ui_torrents.h"

#include "../../../src/clients/nekomimi.h"

namespace Views {

Torrents::Torrents(QWidget *parent)
    : QWidget(parent),
      ui(new Ui::Torrents),
      model(new RSSTableModel(this)),
      timer(new QTimer(this)) {
  ui->setupUi(this);

  model->setList(items);
  timer->setInterval(1000);

  ui->torrentTable->setModel(model);
  ui->torrentTable->horizontalHeader()->setStretchLastSection(true);
  ui->torrentTable->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
  ui->torrentTable->verticalHeader()->setDefaultSectionSize(
      ui->torrentTable->fontMetrics().height() + 8);
  ui->torrentTable->setSelectionBehavior(QAbstractItemView::SelectRows);
  ui->torrentTable->setSelectionMode(QAbstractItemView::SingleSelection);

  connect(ui->refreshButton, &QPushButton::clicked, [this]() {
    refresh = 1;
    timerTick();
  });

  connect(timer, SIGNAL(timeout()), SLOT(timerTick()));

  timer->start();
}

Torrents::~Torrents() { delete ui; }

void Torrents::timerTick() {
  --refresh;

  if (refresh <= 0) {
    fetchTorrents();
    refresh = 900;
  }

  ui->refreshButton->setText(tr("Refresh (%1)").arg(refresh));
}
void Torrents::fetchTorrents() {
  for (auto &&item : items.values()) {
    delete item;
  }

  items = Nekomimi::instance().fetch();

  model->setList(items);
  ui->torrentTable->resizeColumnsToContents();
}

}  // namespace Views
