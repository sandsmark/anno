#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QSettings>
#include <QTimer>
#include <QTreeWidget>
#include <QPainter>
#include <QFileDialog>
#include <QDirIterator>
#include <QDockWidget>
#include <assert.h>

namespace {
    const char* companyName = "Tomaattinen";
    const char* applicationName = "anno";
}

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("anno");

    QSettings settings(companyName, applicationName);
    restoreGeometry(settings.value("mainWindowGeometry").toByteArray());
    restoreState(settings.value("mainWindowState").toByteArray());

    connect(ui->actionOpenFolder, SIGNAL(triggered()), this, SLOT(onOpenFolder()));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    QTimer::singleShot(0, this, SLOT(init()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    QSettings settings(companyName, applicationName);
    settings.setValue("mainWindowGeometry", saveGeometry());
    settings.setValue("mainWindowState", saveState());

    QMainWindow::closeEvent(event);
}

void MainWindow::init()
{
}

void MainWindow::createFileList()
{
    QDockWidget* fileListDockWidget = new QDockWidget(tr("Files"), this);
    fileListDockWidget->setObjectName("Files");

    files = new QTreeWidget(this);

    fileListDockWidget->setWidget(files);
    addDockWidget(Qt::LeftDockWidgetArea, fileListDockWidget);

    files->setColumnCount(1);
    files->setFont(QFont("Arial", 8, 0));

    QStringList columns;
    columns.append(tr("Name"));

    QTreeWidgetItem* headerItem = new QTreeWidgetItem(columns);
    headerItem->setTextAlignment(0, Qt::AlignLeft);
    files->setHeaderItem(headerItem);

    connect(files, SIGNAL(itemClicked(QTreeWidgetItem*,int)), this, SLOT(onFileClicked(QTreeWidgetItem*,int)));
}

void MainWindow::onOpenFolder()
{
    QSettings settings(companyName, applicationName);
    const QString defaultDirectory = settings.value("defaultDirectory").toString();
    const QString dir = QFileDialog::getExistingDirectory(this, tr("Open Folder"), defaultDirectory,
                                                          QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);

    if (!dir.isEmpty()) {
        settings.setValue("defaultDirectory", dir);

        if (!files) {
            createFileList();
        }

        files->clear();

        QList<QTreeWidgetItem *> items;

        QStringList columns;
        while (columns.count() < files->columnCount()) {
            columns.append("");
        }

        QApplication::setOverrideCursor(Qt::WaitCursor);
        QApplication::processEvents(); // actually update the cursor

        QDirIterator it(dir, QStringList() << "*.jpg" << "*.png", QDir::Files, QDirIterator::Subdirectories);
        while (it.hasNext()) {
            QString filename = it.next();
            columns[0] = filename;
            QTreeWidgetItem* item = new QTreeWidgetItem(files, columns);
            items.append(item);
        }

        files->insertTopLevelItems(0, items);

        QApplication::restoreOverrideCursor();
    }
}

void MainWindow::onFileClicked(QTreeWidgetItem* item, int column)
{

}
