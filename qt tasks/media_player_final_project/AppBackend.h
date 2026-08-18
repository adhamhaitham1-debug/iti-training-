#ifndef APPBACKEND_H
#define APPBACKEND_H

#include <QObject>
#include <QSettings>
#include <QStringList>
#include <QFileInfo>

class AppBackend : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList queueFiles READ queueFiles NOTIFY queueChanged)
    Q_PROPERTY(QStringList favoritesFiles READ favoritesFiles NOTIFY favoritesChanged)
    Q_PROPERTY(QStringList downloadsFiles READ downloadsFiles NOTIFY downloadsChanged)

public:
    explicit AppBackend(QObject *parent = nullptr) : QObject(parent) {
        QSettings settings("MyCompany", "QtMediaPlayer");
        m_queueFiles = settings.value("queue").toStringList();
        m_favoritesFiles = settings.value("favorites").toStringList();
        m_downloadsFiles = settings.value("downloads").toStringList();
    }

    ~AppBackend() {
        QSettings settings("MyCompany", "QtMediaPlayer");
        settings.setValue("queue", m_queueFiles);
        settings.setValue("favorites", m_favoritesFiles);
        settings.setValue("downloads", m_downloadsFiles);
    }

    QStringList queueFiles() const { return m_queueFiles; }
    QStringList favoritesFiles() const { return m_favoritesFiles; }
    QStringList downloadsFiles() const { return m_downloadsFiles; }

    Q_INVOKABLE QString getFileName(const QString &filePath) {
        return QFileInfo(filePath).fileName();
    }

    Q_INVOKABLE void addToQueue(const QString &filePath) {
        if (!m_queueFiles.contains(filePath)) {
            m_queueFiles.append(filePath);
            emit queueChanged();
        }
    }

    Q_INVOKABLE void toggleFavorite(const QString &filePath) {
        if (m_favoritesFiles.contains(filePath)) m_favoritesFiles.removeAll(filePath);
        else m_favoritesFiles.append(filePath);
        emit favoritesChanged();
    }

    Q_INVOKABLE void toggleDownload(const QString &filePath) {
        if (m_downloadsFiles.contains(filePath)) m_downloadsFiles.removeAll(filePath);
        else m_downloadsFiles.append(filePath);
        emit downloadsChanged();
    }

signals:
    void queueChanged();
    void favoritesChanged();
    void downloadsChanged();

private:
    QStringList m_queueFiles;
    QStringList m_favoritesFiles;
    QStringList m_downloadsFiles;
};

#endif // APPBACKEND_H
