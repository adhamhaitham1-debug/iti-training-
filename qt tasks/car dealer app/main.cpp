#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QTranslator>
#include <QQmlContext>
#include <QDebug>

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QQmlApplicationEngine engine;

    static QTranslator translator;

    // Connect language changes from QML to C++
    QObject::connect(&engine, &QQmlApplicationEngine::uiLanguageChanged, [&app, &engine]() {
        QString language = engine.uiLanguage();
        qDebug() << "Language changed in QML to:" << language;

        app.removeTranslator(&translator);

        if (language != "en") {
            bool loaded = false;

            // Try loading from Qt 6 resource module paths
            if (!loaded) loaded = translator.load(":/i18n/app_" + language + ".qm");
            if (!loaded) loaded = translator.load(":/qt/qml/shop/i18n/app_" + language + ".qm");
            // Try loading from local file system relative to executable
            if (!loaded) loaded = translator.load("i18n/app_" + language + ".qm");
            if (!loaded) loaded = translator.load("../shop/i18n/app_" + language + ".qm");

            if (loaded) {
                app.installTranslator(&translator);
                qDebug() << "Successfully installed translator for:" << language;
            } else {
                qDebug() << "Failed to load translation file for:" << language;
            }
        }

        // Force QML to re-evaluate all qsTr() strings
        engine.retranslate();
    });

    engine.loadFromModule("shop", "Main");

    return app.exec();
}