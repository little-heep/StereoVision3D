//
// Created by Micheal on 2026/6/12.
//

#include "MainWindow.h"

#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QResizeEvent>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
{
	// Central widget and layout
	QWidget *central = new QWidget(this);
	auto *layout = new QHBoxLayout(central);

	// Left: load button
	btnLoad = new QPushButton(QStringLiteral("加载图片"), central);
	layout->addWidget(btnLoad, 0);

	// Right: image label
	lblImage = new QLabel(central);
	lblImage->setAlignment(Qt::AlignCenter);
	lblImage->setMinimumSize(200, 200);
	lblImage->setStyleSheet("QLabel { background-color: #222; color: #fff; }");
	layout->addWidget(lblImage, 1);

	setCentralWidget(central);

	connect(btnLoad, &QPushButton::clicked, this, &MainWindow::onLoadImage);
}

MainWindow::~MainWindow() = default;

void MainWindow::onLoadImage()
{
	// Read image from data/left.png in color mode
	const std::string path = "D:/QtProject/StereoVision3D/data/left.jpg";
	cv::Mat mat = cv::imread(path, cv::IMREAD_COLOR);
	if (mat.empty()) {
		QMessageBox::warning(this, tr("加载图片失败"), tr("无法读取: %1").arg(QString::fromStdString(path)));
		return;
	}

	currentImage = matToQImage(mat);
	if (currentImage.isNull()) {
		QMessageBox::warning(this, tr("转换失败"), tr("无法将图像转换为 QImage"));
		return;
	}

	// Scale to fit label while keeping aspect ratio
	QPixmap pm = QPixmap::fromImage(currentImage.scaled(lblImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
	lblImage->setPixmap(pm);
}

void MainWindow::resizeEvent(QResizeEvent *event)
{
	QMainWindow::resizeEvent(event);
	if (!currentImage.isNull() && lblImage) {
		QPixmap pm = QPixmap::fromImage(currentImage.scaled(lblImage->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
		lblImage->setPixmap(pm);
	}
}

QImage MainWindow::matToQImage(const cv::Mat &mat)
{
	if (mat.empty()) return QImage();

	// Handle 8-bit, 1/3/4-channel images
	if (mat.type() == CV_8UC3) {
		cv::Mat rgb;
		cv::cvtColor(mat, rgb, cv::COLOR_BGR2RGB);
		return QImage(rgb.data, rgb.cols, rgb.rows, static_cast<int>(rgb.step), QImage::Format_RGB888).copy();
	}

	if (mat.type() == CV_8UC1) {
		return QImage(mat.data, mat.cols, mat.rows, static_cast<int>(mat.step), QImage::Format_Grayscale8).copy();
	}

	if (mat.type() == CV_8UC4) {
		cv::Mat rgba;
		cv::cvtColor(mat, rgba, cv::COLOR_BGRA2RGBA);
		return QImage(rgba.data, rgba.cols, rgba.rows, static_cast<int>(rgba.step), QImage::Format_RGBA8888).copy();
	}

	// Unsupported format
	return QImage();
}
