#ifndef WEBVIEWPAGE_H
#define WEBVIEWPAGE_H

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QWebEngineView>
#include <QWebEngineProfile>
#include <QUrl>
#include "mywebengineview.h"

class WebViewPage : public QWidget {
    Q_OBJECT

public:
    explicit WebViewPage(QWidget *parent = nullptr);
    virtual ~WebViewPage();
    
    void setUrl(const QUrl &url);
    void navigateTo(const QUrl &url);
    void goBack();
    void goHome();
    void setHomeUrl(const QUrl &url);

protected:
    virtual void setupUI();
    virtual void createNavigationBar();
    virtual void createWebView();
    virtual void configureWebView();
    virtual void connectSignals();
    
    QVBoxLayout *mainLayout;
    QWidget *navBar;
    QHBoxLayout *navLayout;
    QPushButton *backButton;
    QPushButton *homeButton;
    QPushButton *loginButton;  // Added login button here
    MyWebEngineView *webView;

    QUrl homeUrl;
    QUrl initialUrl;
};

#endif // WEBVIEWPAGE_H
