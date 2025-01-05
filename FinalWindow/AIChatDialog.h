#ifndef AICHATDIALOG_H
#define AICHATDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QTextEdit>
#include <QListWidget>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QSettings>
#include <QMenu>
#include <QDateEdit>  // 引入 QDateEdit

class AIChatDialog : public QDialog {
    Q_OBJECT

public:
    AIChatDialog(QWidget *parent = nullptr);
    ~AIChatDialog();

private slots:
    void onAskClicked();
    void onNetworkReply(QNetworkReply* reply);
    void onAddTaskClicked();  // 新增任務的槽函數
    void onTaskCompleted(QListWidgetItem* item);  // 標註任務為完成的槽函數
    void onRemoveTask();  // 刪除任務的槽函數
    void onTaskContextMenuRequested(const QPoint &pos);  // 顯示右鍵菜單

private:
    void loadTasks();  // 載入任務
    void saveTasks();  // 儲存任務

    QLineEdit *questionInput;
    QPushButton *askButton;
    QTextEdit *answerOutput;

    QLineEdit *taskInput;  // 用於輸入新任務
    QPushButton *addTaskButton;  // 用於新增任務
    QListWidget *taskListWidget;  // 顯示任務的列表

    QNetworkAccessManager *networkManager;
    QSettings settings;  // 用來儲存任務狀態的 QSettings
    QDateEdit *dateEdit;  // 日期選擇器
};

#endif // AICHATDIALOG_H
