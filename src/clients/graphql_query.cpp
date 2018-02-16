#include "./graphql_query.h"

#include "./anilist.h"

GraphQLQuery::GraphQLQuery(const QString &file) {
  QFile mFile(file);

  if (!mFile.open(QFile::ReadOnly | QFile::Text)) {
    return;
  }

  QTextStream in(&mFile);
  m_query = in.readAll();
  mFile.close();
}

QNetworkReply *GraphQLQuery::query() {
  return AniList::instance().graphql(m_query, m_variables);
}
