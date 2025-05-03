#ifndef YOUTUBEMUSICPAGE_H
#define YOUTUBEMUSICPAGE_H

#include "WebViewPage.h"

class YouTubeMusicPage : public WebViewPage {
    Q_OBJECT

public:
    explicit YouTubeMusicPage(QWidget *parent = nullptr);
    ~YouTubeMusicPage() override;
};

#endif // YOUTUBEMUSICPAGE_H
