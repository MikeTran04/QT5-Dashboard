#ifndef MYWEBENGINEVIEW_H
#define MYWEBENGINEVIEW_H

#include <QWebEngineView>

class MyWebEngineView : public QWebEngineView {
public:
    explicit MyWebEngineView(QWidget *parent = nullptr)
        : QWebEngineView(parent) {}

protected:
    // Override createWindow to force links to load in the same view
    QWebEngineView* createWindow(QWebEnginePage::WebWindowType type) override {
        Q_UNUSED(type);
        return this;
    }
};

#endif // MYWEBENGINEVIEW_H
