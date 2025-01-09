#ifndef FINALWINDOW_H
#define FINALWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QPushButton>
#include <QStatusBar>
#include <QToolBar>
#include <QSqlDatabase>

class FinalWindow : public QMainWindow {
    Q_OBJECT

public:
    FinalWindow(QWidget *parent = nullptr);
    ~FinalWindow();

private:
    QSqlDatabase db;
    QTableWidget *tableWidget;

    bool setupDatabase();
    void loadTodoItems();
    void addTodoItem();
};

#endif // FINALWINDOW_H
