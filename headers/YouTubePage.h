#ifndef YOUTUBEPAGE_H
#define YOUTUBEPAGE_H

#include "WebViewPage.h"

class YouTubePage : public WebViewPage {
    Q_OBJECT

public:
    explicit YouTubePage(QWidget *parent = nullptr);
    ~YouTubePage() override;
};

#endif // YOUTUBEPAGE_H
