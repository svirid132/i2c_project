#include "i2c_controller.h"

#include <QGuiApplication>
#include <QQmlApplicationEngine>

//#define DEV_ADDRESS 0x57
//#define DEV_REGISTERS_NUM 6

int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif
    QGuiApplication app(argc, argv);

    qmlRegisterType<I2C_Controller>("Controller", 1, 0, "I2C_Controller");
    qmlRegisterType<I2C_Model>("Model", 1, 0, "I2C_Model");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
