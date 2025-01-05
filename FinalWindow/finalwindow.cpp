#include "FinalWindow.h"
#include <QMenuBar>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QInputDialog>
#include <QSqlDatabase>

FinalWindow::FinalWindow(QWidget *parent) : QMainWindow(parent) {
    // 初始化界面
    setWindowTitle("SQLite 整合示例");
    resize(600, 400);

    // 初始化數據庫
    if (!setupDatabase()) {
        QMessageBox::critical(this, "錯誤", "無法初始化數據庫！");
        return;
    }

    // 創建主界面
    QWidget *centralWidget = new QWidget(this);
    QVBoxLayout *layout = new QVBoxLayout();

    // 創建顯示數據的表格
    tableWidget = new QTableWidget();
    tableWidget->setColumnCount(2);
    tableWidget->setHorizontalHeaderLabels({"ID", "待辦事項"});
    layout->addWidget(tableWidget);

    // 創建按鈕
    QPushButton *addButton = new QPushButton("添加一條待辦事項");
    layout->addWidget(addButton);

    QPushButton *loadButton = new QPushButton("重新加載數據");
    layout->addWidget(loadButton);

    // 設置中央部件
    centralWidget->setLayout(layout);
    setCentralWidget(centralWidget);

    // 連接按鈕操作
    connect(addButton, &QPushButton::clicked, this, &FinalWindow::addTodoItem);
    connect(loadButton, &QPushButton::clicked, this, &FinalWindow::loadTodoItems);

    // 加載數據
    loadTodoItems();
}


FinalWindow::~FinalWindow() {
    if (db.isOpen()) {
        db.close();
    }
}

bool FinalWindow::setupDatabase() {
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("todoapp.db");

    if (!db.open()) {
        qCritical() << "無法打開數據庫：" << db.lastError().text();
        return false;
    }

    // 創建數據表
    QSqlQuery query;
    QString createTableQuery =
        "CREATE TABLE IF NOT EXISTS todo ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        "description TEXT NOT NULL)";
    if (!query.exec(createTableQuery)) {
        qCritical() << "創建數據表失敗：" << query.lastError().text();
        return false;
    }

    return true;
}

void FinalWindow::loadTodoItems() {
    tableWidget->setRowCount(0);

    // 加載數據
    QSqlQuery query("SELECT id, description FROM todo");
    int row = 0;
    while (query.next()) {
        tableWidget->insertRow(row);
        tableWidget->setItem(row, 0, new QTableWidgetItem(query.value(0).toString()));
        tableWidget->setItem(row, 1, new QTableWidgetItem(query.value(1).toString()));
        row++;
    }
}

void FinalWindow::addTodoItem() {
    QSqlQuery query;
    if (!query.exec("INSERT INTO todo (description) VALUES ('新待辦事項')")) {
        QMessageBox::warning(this, "錯誤", "插入數據失敗：" + query.lastError().text());
        return;
    }

    // 更新顯示
    loadTodoItems();
}
