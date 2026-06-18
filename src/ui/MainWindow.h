//
// Created by Micheal on 2026/6/12.
//

#ifndef STEREOVISION3D_MAINWINDOW_H
#define STEREOVISION3D_MAINWINDOW_H

#include <QMainWindow>
#include <QImage>
#include <QPushButton>
#include <QLabel>

namespace cv { class Mat; }

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

private slots:
    void onLoadImage();

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    QPushButton *btnLoad{nullptr};
    QLabel *lblImage{nullptr};
    QImage currentImage;

    // Convert an OpenCV cv::Mat to a QImage. Handles common 8-bit formats.
    static QImage matToQImage(const cv::Mat &mat);
};


#endif //STEREOVISION3D_MAINWINDOW_H