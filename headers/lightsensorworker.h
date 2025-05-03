#ifndef LIGHTSENSORWORKER_H
#define LIGHTSENSORWORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QProcess>

class LightSensorWorker : public QObject {
    Q_OBJECT

public:
    explicit LightSensorWorker(QObject *parent = nullptr) : QObject(parent) {}

public slots:
    inline void readLightSensor();  // Mark the function as inline

signals:
    void lightLevelUpdated(int brightness);
};

// Inline implementation of readLightSensor
inline void LightSensorWorker::readLightSensor() {
    QProcess *process = new QProcess(this);
    process->start("python3", QStringList() << "/home/dash/Documents/group19/light_sensor.py");

    if (!process->waitForStarted()) {
        qDebug() << "Failed to start light sensor script!";
        emit lightLevelUpdated(-1);  // Indicate error condition
        return;
    }

    connect(process, &QProcess::readyReadStandardOutput, [process, this]() {
        QByteArray output = process->readAllStandardOutput();
        QList<QByteArray> lines = output.split('\n');  // Handle multiple lines of output
        for (const QByteArray &line : lines) {
            if (!line.trimmed().isEmpty()) {
                int brightness = line.trimmed().toInt();
                qDebug() << "Brightness from sensor:" << brightness;  // Debug log
                emit lightLevelUpdated(brightness);
            }
        }
    });

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            [process, this](int exitCode, QProcess::ExitStatus exitStatus) {
                if (exitStatus != QProcess::NormalExit || exitCode != 0) {
                    qDebug() << "Light sensor script failed with exit code:" << exitCode;
                    emit lightLevelUpdated(-1);  // Indicate error condition
                }
                process->deleteLater();
            });
}

#endif // LIGHTSENSORWORKER_H
