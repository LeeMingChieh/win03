#include "CalendarWidget.h"
#include "AIChatDialog.h"
#include <QVBoxLayout>

CalendarWidget::CalendarWidget(QWidget* parent) : QWidget(parent) {
    QVBoxLayout* layout = new QVBoxLayout(this);

    calendar = new QCalendarWidget(this);
    QPushButton* chatButton = new QPushButton("新增任務且可以詢問AI", this);

    layout->addWidget(calendar);
    layout->addWidget(chatButton);

    connect(chatButton, &QPushButton::clicked, this, &CalendarWidget::onChatButtonClicked);
}

void CalendarWidget::onChatButtonClicked() {
    AIChatDialog chatDialog(this);
    chatDialog.exec();
}
