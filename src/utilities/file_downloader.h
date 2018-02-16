#ifndef SRC_UTILITIES_FILEDOWNLOADER_H__
#define SRC_UTILITIES_FILEDOWNLOADER_H__

#include <QByteArray>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QObject>

class FileDownloader : public QObject {
  Q_OBJECT

 public:
  explicit FileDownloader(QUrl url, QObject *parent = 0);
  virtual ~FileDownloader();
  QByteArray downloadedData() const;

 signals:
  void downloaded();

 private slots:
  void fileDownloaded(QNetworkReply *reply);

 private:
  QNetworkAccessManager m_WebCtrl;
  QByteArray m_DownloadedData;
};

#endif  // SRC_UTILITIES_FILEDOWNLOADER_H__
