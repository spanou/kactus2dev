#include "PoshScriptProcess.h"
#include "mainwindow.h"

#include <iostream>

PoshScriptProcess::PoshScriptProcess(QObject *parent) :
    QObject(parent){

    connect(&theProcess, SIGNAL(finished(int, QProcess::ExitStatus)),
        this, SLOT(onFinished(int, QProcess::ExitStatus)));

    connect(&theProcess, SIGNAL(started()), this, SLOT(onStarted()));

    connect(&theProcess, SIGNAL(errorOccurred(QProcess::ProcessError)),
        this, SLOT(onErrorOccurred(QProcess::ProcessError)));

    connect(&theProcess, SIGNAL(readyReadStandardError()),
        this, SLOT(onReadyReadStandardOutput()));

    connect(&theProcess, SIGNAL(readyReadStandardError()),
            this, SLOT(onReadyReadStandardError()));
}

PoshScriptProcess::~PoshScriptProcess(){
}

QProcess& PoshScriptProcess::getProcess(){
    return(theProcess);
}

void PoshScriptProcess::onFinished(int exitCode, QProcess::ExitStatus exitStatus){
    // Avoid Compiler Warnings, Has No Effect
    exitStatus = exitStatus;

    QString output(theProcess.readAllStandardOutput().constData());
    emit ((MainWindow*)parent())->noticeMessage(output);

    QString theExitCode = QString("Process Completed With Exit Code: %1").
        arg(QString::number(exitCode));

    emit ((MainWindow*)parent())->noticeMessage(theExitCode);
}

void PoshScriptProcess::onErrorOccurred(QProcess::ProcessError error){
    QString failureReason("");

    switch(error){
        case QProcess::ProcessError::FailedToStart:
            failureReason = "Failed To Start The Process";
            break;
        case QProcess::ProcessError::Crashed:
            failureReason = "The Process Crashed";
            break;
        case QProcess::ProcessError::Timedout:
            failureReason = "The Process Timed Out";
            break;
        case QProcess::ProcessError::WriteError:
            failureReason = "Failed To Write To The Process";
            break;
        case QProcess::ProcessError::ReadError:
            failureReason = "Failed to Read From The Process";
            break;
        case QProcess::ProcessError::UnknownError:
        default:
            failureReason = "The Process Exhibitted An Unknown Failure";
    }

    emit ((MainWindow*)parent())->errorMessage(failureReason);
}

void PoshScriptProcess::onReadyReadStandardOutput(){
    emit ((MainWindow*)parent())->noticeMessage(
        theProcess.readAllStandardOutput().constData());
}

void PoshScriptProcess::onReadyReadStandardError(){
    emit ((MainWindow*)parent())->errorMessage(
        theProcess.readAllStandardError().constData());
}

void PoshScriptProcess::onStarted(){
    QString command = QString("\n\n\nProcess Launched: %1 %2").
        arg(theProcess.program(), theProcess.arguments().join(" "));

    emit ((MainWindow*)parent())->noticeMessage(command);
}
