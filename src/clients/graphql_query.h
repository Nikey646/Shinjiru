#ifndef SRC_CLIENTS_GRAPHQL_QUERY_H__
#define SRC_CLIENTS_GRAPHQL_QUERY_H__

#include <QJsonObject>
#include <QNetworkReply>
#include <QString>
#include <QVariant>

class GraphQLQuery {
 public:
  GraphQLQuery(const QString &file);

  QNetworkReply *query();

  template <typename T>
  void set(const QString &key, const T &value) {
    m_variables[key] = value;
  }

 private:
  QJsonObject m_variables;
  QString m_query;
};

#endif  // SRC_CLIENTS_GRAPHQL_QUERY_H__
