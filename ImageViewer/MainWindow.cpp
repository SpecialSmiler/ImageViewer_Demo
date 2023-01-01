#include <QHBoxLayout>
#include <QPushButton>
#include <QFileDialog>
#include <QImage>
#include <QGraphicsItem>

#include "MainWindow.h"
#include "View.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), m_scene(new QGraphicsScene(this))
{
    QWidget* centerWidget = new QWidget(this);
    centerWidget->setMinimumSize(400, 300);
    setCentralWidget(centerWidget);

    QHBoxLayout* topLayout = new QHBoxLayout();
    QPushButton* openImageBtn = new QPushButton("Open Image", this);
    QPushButton* resetZoomBtn = new QPushButton("Reset Zoom", this);
    QPushButton* loadPresetBtn = new QPushButton("Load Preset", this);

    topLayout->addWidget(openImageBtn);
    topLayout->addWidget(resetZoomBtn);
    topLayout->addWidget(loadPresetBtn);
    topLayout->addStretch();

    m_view = new View("Image View", this);
    GraphicsView* graphicsView = m_view->view();
    //view->view()->setScene(scene);
    graphicsView->setScene(m_scene);

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->addLayout(topLayout);
    mainLayout->addWidget(m_view);
    mainLayout->setStretch(0, 1);
    mainLayout->setStretch(0, 10);
    centerWidget->setLayout(mainLayout);

    connect(openImageBtn, &QPushButton::clicked, this, &MainWindow::loadImageFromFile);
    connect(resetZoomBtn, &QPushButton::clicked, graphicsView, &GraphicsView::resetZoom);
    connect(loadPresetBtn, &QPushButton::clicked, this, &MainWindow::loadPreset);
}

MainWindow::~MainWindow()
{}

void MainWindow::loadPreset()
{
    QString filePath = QStringLiteral("D:/Pictures/14faf5deb48f8c54511d076b2d292df5e2fe7fd0.jpg");
    loadImageToScene(filePath);
}

void MainWindow::loadImageFromFile()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Image"));
    loadImageToScene(filePath);
}

void MainWindow::loadImageToScene(QString fileName)
{
    QImage image;
    if (image.load(fileName))
    {
        m_scene->clear();

        QPixmap underLayerPixmap(image.size().width() * 2, image.size().height() * 2);
        underLayerPixmap.fill(QColor::fromRgb(48, 48, 48));
        QGraphicsPixmapItem* underLayerPixmapItem = new QGraphicsPixmapItem(underLayerPixmap);
        int offsetX = -image.width() / 2;
        int offsetY = -image.height() / 2;
        underLayerPixmapItem->setPos(offsetX, offsetY);

        m_scene->addItem(underLayerPixmapItem);
        m_scene->addPixmap(QPixmap::fromImage(image));

        m_scene->setSceneRect(offsetX, offsetY, underLayerPixmap.width(), underLayerPixmap.height());

        m_view->view()->resetZoom();
        m_view->view()->focusToCenter();
    }
}

