#ifndef POSHSCRIPTPROCESS_H
#define POSHSCRIPTPROCESS_H

#include <QProcess>
#include <QObject>

class PoshScriptProcess : public QObject
{
    Q_OBJECT

public:
    explicit PoshScriptProcess(QObject *parent = nullptr);
    ~PoshScriptProcess();

    QProcess& getProcess();

public slots:
    void onFinished(int exitCode, QProcess::ExitStatus exitStatus);
    void onErrorOccurred(QProcess::ProcessError error);
    void onReadyReadStandardOutput();
    void onReadyReadStandardError();
    void onStarted();

private:
    QProcess theProcess;
};

#endif // POSHSCRIPTPROCESS_H
