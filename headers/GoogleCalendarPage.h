#ifndef GOOGLECALENDARPAGE_H
#define GOOGLECALENDARPAGE_H

#include "WebViewPage.h"

class GoogleCalendarPage : public WebViewPage {
    Q_OBJECT

public:
    explicit GoogleCalendarPage(QWidget *parent = nullptr);
    ~GoogleCalendarPage() override;
};

#endif // GOOGLECALENDARPAGE_H