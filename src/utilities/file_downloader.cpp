#include "./file_downloader.h"

FileDownloader::FileDownloader(QUrl url, QObject *parent) : QObject(parent) {
  connect(&m_WebCtrl, SIGNAL(finished(QNetworkReply *)), this,
          SLOT(fileDownloaded(QNetworkReply *)));

  QNetworkRequest request(url);
  m_WebCtrl.get(request);
}

FileDownloader::~FileDownloader() {}

void FileDownloader::fileDownloaded(QNetworkReply *reply) {
  m_DownloadedData = reply->readAll();

  reply->deleteLater();
  emit downloaded();
}

QByteArray FileDownloader::downloadedData() const { return m_DownloadedData; }
