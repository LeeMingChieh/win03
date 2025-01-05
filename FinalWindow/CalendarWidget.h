#ifndef CALENDARWIDGET_H
#define CALENDARWIDGET_H

#include <QWidget>
#include <QCalendarWidget>
#include <QPushButton>

class CalendarWidget : public QWidget {
    Q_OBJECT

public:
    explicit CalendarWidget(QWidget* parent = nullptr);

private slots:
    void onChatButtonClicked();

private:
    QCalendarWidget* calendar;
};

#endif // CALENDARWIDGET_H
