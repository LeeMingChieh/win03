#include "AIChatDialog.h"
#include <QVBoxLayout>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QNetworkRequest>
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QCheckBox>
#include <QListWidgetItem>
#include <QBrush>
#include <QColor>
#include <QSettings>
#include <QMenu>
#include <QDateEdit>  // 引入 QDateEdit
#include <QDate>

AIChatDialog::AIChatDialog(QWidget *parent) :
    QDialog(parent),
    settings("MyCompany", "TaskManager") {  // 設定 QSettings 儲存位置
    setWindowTitle("AI Chat");
    setFixedSize(400, 500);

    // 問題輸入和回應顯示
    questionInput = new QLineEdit(this);
    questionInput->setPlaceholderText("輸入問題...");

    askButton = new QPushButton("問", this);
    answerOutput = new QTextEdit(this);
    answerOutput->setReadOnly(true);

    // 任務輸入和任務列表
    taskInput = new QLineEdit(this);
    taskInput->setPlaceholderText("輸入任務...");

    addTaskButton = new QPushButton("新增任務", this);
    taskListWidget = new QListWidget(this);

    // 日期選擇器
    dateEdit = new QDateEdit (QDate::currentDate(), this);  // 預設為當前日期
    dateEdit->setDisplayFormat("yyyy-MM-dd");  // 設定日期格式
    dateEdit->setCalendarPopup(true);  // 顯示行事曆彈出選擇

    // 啟用右鍵菜單
    taskListWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(taskListWidget, &QListWidget::customContextMenuRequested, this, &AIChatDialog::onTaskContextMenuRequested);

    // 設定佈局
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(questionInput);
    layout->addWidget(askButton);
    layout->addWidget(answerOutput);

    layout->addWidget(taskInput);
    layout->addWidget(dateEdit);  // 加入日期選擇控件
    layout->addWidget(addTaskButton);
    layout->addWidget(taskListWidget);
    setLayout(layout);

    // 連接按鈕事件
    connect(askButton, &QPushButton::clicked, this, &AIChatDialog::onAskClicked);
    connect(addTaskButton, &QPushButton::clicked, this, &AIChatDialog::onAddTaskClicked);
    connect(taskListWidget, &QListWidget::itemChanged, this, &AIChatDialog::onTaskCompleted);

    // 設定網絡管理器
    networkManager = new QNetworkAccessManager(this);
    connect(networkManager, &QNetworkAccessManager::finished, this, &AIChatDialog::onNetworkReply);

    // 載入任務列表
    loadTasks();
}

AIChatDialog::~AIChatDialog() {
    // 儲存任務列表
    saveTasks();
}

void AIChatDialog::onAskClicked() {
    QString question = questionInput->text();
    if (!question.isEmpty()) {
        // 模擬AI回應
        answerOutput->append("AI: " + question + " is a good question!");
    }
    /*QString question = questionInput->text();
    if (question.isEmpty()) {
        return;
    }

    // 設定 API 請求內容，使用 gpt-3.5-turbo 模型
    QJsonObject json;
    json["model"] = "gpt-3.5-turbo";  // 使用 gpt-3.5-turbo 模型
    QJsonArray messages;
    QJsonObject userMessage;
    userMessage["role"] = "user";
    userMessage["content"] = question;
    messages.append(userMessage);
    json["messages"] = messages;

    QNetworkRequest request(QUrl("https://api.openai.com/v1/chat/completions"));
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setRawHeader("Authorization", "Bearer YOUR_API_KEY");  // 使用你的 API 金鑰

    // 發送 POST 請求
    networkManager->post(request, QJsonDocument(json).toJson());*/
}

void AIChatDialog::onNetworkReply(QNetworkReply* reply) {
    if (reply->error() != QNetworkReply::NoError) {
        // 輸出錯誤訊息，了解發生了什麼問題
        answerOutput->append("Error: " + reply->errorString());
        answerOutput->append("Details: " + reply->readAll());
    } else {
        QByteArray responseData = reply->readAll();
        QJsonDocument jsonDoc = QJsonDocument::fromJson(responseData);
        if (jsonDoc.isObject()) {
            QJsonObject jsonObject = jsonDoc.object();
            if (jsonObject.contains("choices") && jsonObject["choices"].isArray()) {
                QString aiResponse = jsonObject["choices"].toArray()[0].toObject()["message"].toObject()["content"].toString();
                answerOutput->append("AI: " + aiResponse);
            } else {
                answerOutput->append("Unexpected response format.");
            }
        } else {
            answerOutput->append("Invalid JSON response.");
        }
    }
    reply->deleteLater();
}

// 新增任務
void AIChatDialog::onAddTaskClicked() {
    QString taskText = taskInput->text();
    if (!taskText.isEmpty()) {
        // 取得選擇的日期
        QString selectedDate = dateEdit->date().toString("yyyy-MM-dd");

        // 創建新任務並顯示日期
        QListWidgetItem *newItem = new QListWidgetItem(taskText + " - " + selectedDate);
        newItem->setCheckState(Qt::Unchecked);  // 預設為未完成
        taskListWidget->addItem(newItem);
        taskInput->clear();
    }
}

// 標註任務為完成，並改變顏色
void AIChatDialog::onTaskCompleted(QListWidgetItem* item) {
    if (item->checkState() == Qt::Checked) {
        item->setBackground(QBrush(QColor(Qt::green)));  // 任務完成，顯示綠色
        //item->setText(item->text().remove(" (未完成)") + " (已完成)");  // 顯示 "已完成"
    } else {
        item->setBackground(QBrush(QColor(Qt::white)));  // 任務未完成，顯示白色
        //item->setText(item->text().remove(" (已完成)").remove(" (未完成)"));  // 顯示 "未完成"
    }
}

// 顯示右鍵菜單
void AIChatDialog::onTaskContextMenuRequested(const QPoint &pos) {
    QListWidgetItem* item = taskListWidget->itemAt(pos);
    if (item) {
        QMenu contextMenu(this);
        QAction* removeAction = contextMenu.addAction("刪除任務");
        connect(removeAction, &QAction::triggered, this, &AIChatDialog::onRemoveTask);
        contextMenu.exec(taskListWidget->mapToGlobal(pos));
    }
}

// 刪除任務
void AIChatDialog::onRemoveTask() {
    QListWidgetItem* item = taskListWidget->currentItem();
    if (item) {
        delete item;  // 刪除當前選中的任務
    }
}

// 載入任務列表
void AIChatDialog::loadTasks() {
    int taskCount = settings.beginReadArray("tasks");
    for (int i = 0; i < taskCount; ++i) {
        settings.setArrayIndex(i);
        QString taskText = settings.value("taskText").toString();
        bool isChecked = settings.value("isChecked").toBool();
        QString taskDate = settings.value("taskDate").toString();

        // 顯示日期
        QListWidgetItem *item = new QListWidgetItem(taskText + " - " + taskDate);
        item->setCheckState(isChecked ? Qt::Checked : Qt::Unchecked);
        item->setBackground(isChecked ? QBrush(QColor(Qt::green)) : QBrush(QColor(Qt::white)));
        item->setText(isChecked ? taskText + " (已完成) - " + taskDate : taskText + " (未完成) - " + taskDate);  // 顯示狀態和日期
        taskListWidget->addItem(item);
    }
    settings.endArray();
}

// 儲存任務列表
void AIChatDialog::saveTasks() {
    settings.beginWriteArray("tasks");
    for (int i = 0; i < taskListWidget->count(); ++i) {
        QListWidgetItem *item = taskListWidget->item(i);
        settings.setArrayIndex(i);
        settings.setValue("taskText", item->text());
        settings.setValue("isChecked", item->checkState() == Qt::Checked);

        // 儲存任務的日期
        QString date = item->text().section('-', 1, 1).trimmed();
        settings.setValue("taskDate", date);
    }
    settings.endArray();
}
